from . import _core
from ._core import Message, MessageType, NmeaException

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
