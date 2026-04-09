"""
Python bindings for nmealib
"""
from __future__ import annotations
import datetime
import typing
from . import nmea0183
from . import nmea2000
__all__: list[str] = ['FrameTooLongException', 'InvalidCanFrameException', 'InvalidPgnException', 'InvalidStartCharacterException', 'Message', 'MessageType', 'NMEA0183', 'NMEA2000', 'NmeaException', 'NoChecksumException', 'NoEndlineException', 'NotAPBException', 'NotDBTException', 'NotDPTException', 'NotEnoughFieldsException', 'NotGGAException', 'NotGLLException', 'NotGSAException', 'NotGSVException', 'NotHDGException', 'NotHDMException', 'NotMTWException', 'NotMWVException', 'NotRMAException', 'NotRMBException', 'NotRMCException', 'NotVHWException', 'NotVLWException', 'NotVTGException', 'NotVWRException', 'NotXDRException', 'NotXTEException', 'NotZDAException', 'TooLongSentenceException', 'Unknown', 'nmea0183', 'nmea2000']
class FrameTooLongException(NmeaException):
    pass
class InvalidCanFrameException(NmeaException):
    pass
class InvalidPgnException(NmeaException):
    pass
class InvalidStartCharacterException(NmeaException):
    pass
class Message:
    @staticmethod
    def type_to_string(message_type: MessageType) -> str:
        ...
    def clone(self) -> Message:
        ...
    def get_raw_data(self) -> str:
        ...
    def get_timestamp(self) -> datetime.datetime:
        ...
    def get_type(self) -> MessageType:
        ...
    def serialize(self) -> str:
        ...
    def validate(self) -> bool:
        ...
class MessageType:
    """
    Members:
    
      Unknown
    
      NMEA0183
    
      NMEA2000
    """
    NMEA0183: typing.ClassVar[MessageType]  # value = <MessageType.NMEA0183: 1>
    NMEA2000: typing.ClassVar[MessageType]  # value = <MessageType.NMEA2000: 2>
    Unknown: typing.ClassVar[MessageType]  # value = <MessageType.Unknown: 0>
    __members__: typing.ClassVar[dict[str, MessageType]]  # value = {'Unknown': <MessageType.Unknown: 0>, 'NMEA0183': <MessageType.NMEA0183: 1>, 'NMEA2000': <MessageType.NMEA2000: 2>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: typing.SupportsInt | typing.SupportsIndex) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: typing.SupportsInt | typing.SupportsIndex) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class NmeaException(Exception):
    pass
class NoChecksumException(NmeaException):
    pass
class NoEndlineException(NmeaException):
    pass
class NotAPBException(NmeaException):
    pass
class NotDBTException(NmeaException):
    pass
class NotDPTException(NmeaException):
    pass
class NotEnoughFieldsException(NmeaException):
    pass
class NotGGAException(NmeaException):
    pass
class NotGLLException(NmeaException):
    pass
class NotGSAException(NmeaException):
    pass
class NotGSVException(NmeaException):
    pass
class NotHDGException(NmeaException):
    pass
class NotHDMException(NmeaException):
    pass
class NotMTWException(NmeaException):
    pass
class NotMWVException(NmeaException):
    pass
class NotRMAException(NmeaException):
    pass
class NotRMBException(NmeaException):
    pass
class NotRMCException(NmeaException):
    pass
class NotVHWException(NmeaException):
    pass
class NotVLWException(NmeaException):
    pass
class NotVTGException(NmeaException):
    pass
class NotVWRException(NmeaException):
    pass
class NotXDRException(NmeaException):
    pass
class NotXTEException(NmeaException):
    pass
class NotZDAException(NmeaException):
    pass
class TooLongSentenceException(NmeaException):
    pass
NMEA0183: MessageType  # value = <MessageType.NMEA0183: 1>
NMEA2000: MessageType  # value = <MessageType.NMEA2000: 2>
Unknown: MessageType  # value = <MessageType.Unknown: 0>
__version__: str = '0.6.1'
