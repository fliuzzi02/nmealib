# NMEA 2000 API Index

This index lists all supported NMEA 2000 PGN classes.

| PGN | Description |
|---|---|
| [PGN127245](PGN127245.md) | Rudder angle/order and rudder direction status. |
| [PGN127250](PGN127250.md) | Vessel heading with deviation and variation. |
| [PGN127251](PGN127251.md) | Rate of turn report. |
| [PGN127257](PGN127257.md) | Attitude report (yaw, pitch, roll). |
| [PGN128259](PGN128259.md) | Speed report (water and ground referenced). |
| [PGN129025](PGN129025.md) | Position report (latitude and longitude). |
| [PGN129026](PGN129026.md) | COG and SOG rapid update. |
| [PGN130306](PGN130306.md) | Wind data (speed, direction, and reference). |

## NMEA 2000 Custom Data Types

| Data Type | Stored Type | Representing Type | Units | Resolution | Range |
|---|---|---|---|---|---|
| Distance | `uint32_t` | `float` | meters (m) | 0.01 m | 0 to 4.295e7 m |
| Speed | `uint16_t` | `float` | meters per second (m/s) | 0.01 m/s | 0 to 655.32 m/s |
| HalfByte | `uint8_t` | `uint8_t` | unitless | 1 | 0 to 15 |
| Byte | `uint8_t` | `uint8_t` | unitless | 1 | 0 to 255 |
| Angle | `uint16_t` | `float` | radians (rad) | 0.0001 rad | 0 to 2*pi rad |
| SignedAngle | `uint16_t` | `float` | radians (rad) | 0.0001 rad | -pi to pi rad |
| Latitude | `int32_t` | `float` | degrees (deg) | 1e-7 deg | -90 to 90 deg |
| Longitude | `int32_t` | `float` | degrees (deg) | 1e-7 deg | -180 to 180 deg |
| AngularRate | `int32_t` | `double` | radians per second (rad/s) | 3.125e-8 rad/s | -67 to 67 rad/s |