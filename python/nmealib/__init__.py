import importlib

from . import _core as _core

# In source-tree test runs, nmealib/_core may be a stubs-only package while
# the compiled extension is built as top-level "_core" in the build directory.
if not hasattr(_core, "Message"):
    _core = importlib.import_module("_core")

Message = _core.Message
MessageType = _core.MessageType
NmeaException = _core.NmeaException

__version__ = getattr(_core, "__version__", "0.0.0")

nmea0183 = _core.nmea0183
nmea2000 = _core.nmea2000


__all__ = [
    "Message",
    "MessageType",
    "NmeaException",
    "nmea0183",
    "nmea2000",
    "__version__",
]
