#!/usr/bin/env python3
"""Generate pybind11 stubs for the nmealib Python extension.

This script uses pybind11-stubgen against the compiled extension module and
copies the generated stubs into the source package so editors (Pylance/Pyright)
can provide completions and signatures.
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


def _parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate Python stubs for nmealib")
    parser.add_argument(
        "--module",
        default="nmealib._core",
        help="Importable extension module name for pybind11-stubgen",
    )
    parser.add_argument(
        "--package-dir",
        default="python/nmealib",
        help="Path to the nmealib source package directory",
    )
    parser.add_argument(
        "--python",
        default=sys.executable,
        help="Python executable used to run pybind11-stubgen",
    )
    return parser.parse_args()


def _write_top_level_stub(package_dir: Path) -> None:
    init_pyi = package_dir / "__init__.pyi"
    init_pyi.write_text(
        """from __future__ import annotations

from ._core import Message as Message
from ._core import MessageType as MessageType
from ._core import NmeaException as NmeaException
from ._core import nmea0183 as nmea0183
from ._core import nmea2000 as nmea2000

__version__: str

__all__: list[str]
""",
        encoding="utf-8",
    )


def _ensure_py_typed(package_dir: Path) -> None:
    py_typed = package_dir / "py.typed"
    if not py_typed.exists():
        py_typed.write_text("", encoding="utf-8")


def _copy_generated_module_stubs(tmpdir: Path, module_name: str, package_dir: Path) -> None:
    module_path = Path(*module_name.split("."))
    generated = tmpdir / module_path
    if not generated.exists():
        raise FileNotFoundError(f"Generated stubs were not found at {generated}")

    target = package_dir / "_core"
    if target.exists():
        shutil.rmtree(target)

    if generated.is_dir():
        shutil.copytree(generated, target)
        return

    target.mkdir(parents=True, exist_ok=True)
    shutil.copy2(generated, target / "__init__.pyi")


def main() -> int:
    args = _parse_args()
    package_dir = Path(args.package_dir).resolve()
    package_dir.mkdir(parents=True, exist_ok=True)

    with tempfile.TemporaryDirectory(prefix="nmealib-stubs-") as tmp:
        tmpdir = Path(tmp)
        cmd = [
            args.python,
            "-m",
            "pybind11_stubgen",
            args.module,
            "-o",
            str(tmpdir),
            "--ignore-all-errors",
        ]
        subprocess.run(cmd, check=True)
        _copy_generated_module_stubs(tmpdir, args.module, package_dir)

    _write_top_level_stub(package_dir)
    _ensure_py_typed(package_dir)

    print(f"Generated stubs for {args.module} in {package_dir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
