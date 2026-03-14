#!/usr/bin/env python3

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path


VERSION_PATTERN = re.compile(r"^[0-9]+\.[0-9]+\.[0-9]+$")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Update VERSION and library.json from an explicit release version."
    )
    parser.add_argument("version", help="Release version in X.Y.Z format.")
    return parser.parse_args()


def update_repo_files(version: str) -> None:
    version_path = Path("VERSION")
    library_json_path = Path("library.json")

    version_path.write_text(f"{version}\n", encoding="utf-8")

    data = json.loads(library_json_path.read_text(encoding="utf-8"))
    data["version"] = version
    library_json_path.write_text(json.dumps(data, indent=2) + "\n", encoding="utf-8")


def main() -> int:
    args = parse_args()

    if not VERSION_PATTERN.fullmatch(args.version):
        print(f"Invalid version: {args.version}", file=sys.stderr)
        return 1

    update_repo_files(args.version)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())