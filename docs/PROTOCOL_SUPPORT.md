# Protocol Support Matrix

This page tracks message/PGN support and implementation details.

## NMEA 0183

| Message | Implemented | Tested| Notes |
|---|---|---|---|
| [APB](#apb--autopilot-sentence-b) | ✅ Yes | ✅ Yes | Autopilot Sentence "B" |
| [DBT](#dbt--depth-below-transducer) | ✅ Yes | ✅ Yes | Depth Below Transducer |
| [DPT](#dpt--depth-of-water) | ✅ Yes | ✅ Yes | Depth of Water |
| [GGA](#gga--global-positioning-system-fix-data) | ✅ Yes | ✅ Yes | Global Positioning System Fix Data |
| [GLL](#gll--geographic-position---latitudelongitude) | ✅ Yes | ✅ Yes | Geographic Position |
| [GSA](#gsa--gps-dop-and-active-satellites) | ✅ Yes | ✅ Yes | GPS DOP and Active Satellites |
| [GSV](#gsv--satellites-in-view) | ✅ Yes | ✅ Yes | GPS Satellites in View |
| [HDG](#hdg--heading-deviation--variation) | ✅ Yes | ✅ Yes | Heading, Deviation and Variation |
| [HDM](#hdm--heading-magnetic) | ✅ Yes | ✅ Yes | Heading, Magnetic |
| [MTW](#mtw--mean-temperature-of-water) | ✅ Yes | ✅ Yes | Mean Temperature of Water |
| [MWV](#mwv--wind-speed-and-angle) | ✅ Yes | ✅ Yes | Wind Speed and Angle |
| [RMA](#rma--recommended-minimum-navigation-information) | ✅ Yes | ✅ Yes | Recommended Minimum Specific Loran-C Data |
| [RMB](#rmb--recommended-minimum-navigation-information) | ✅ Yes | ✅ Yes | Recommended Minimum Navigation Information |
| [RMC](#rmc--recommended-minimum-navigation-information) | ✅ Yes | ✅ Yes | Recommended Minimum Navigation Data |
| [VHW](#vhw--water-speed-and-heading) | ✅ Yes | ✅ Yes | Water Speed and Heading |
| [VLW](#vlw--distance-traveled-through-water) | ✅ Yes | ✅ Yes | Distance Traveled Through Water |
| [VTG](#vtg--track-made-good-and-ground-speed) | ✅ Yes | ✅ Yes | Course Over Ground and Ground Speed |
| [VWR](#vwr--relative-wind-speed-and-angle) | ✅ Yes | ✅ Yes | Relative Wind Speed and Angle |
| [XDR](#xdr--transducer-measurement) | ✅ Yes | ✅ Yes | Transducer Measurements |
| [XTE](#xte--cross-track-error-measured) | ✅ Yes | ✅ Yes | Cross-Track Error, Measured |
| [ZDA](#zda--time--date-utc-day-month-year-local-time-zone) | ✅ Yes | ✅ Yes | Time and Date |

## NMEA 2000

| Message / Transport | Implemented | Tested | Notes |
|---|---|---|---|
| [Single-frame messages](#nmea-2000-single-frame-messages) | ✅ Yes | ❌ No | Standard one-frame CAN payloads |
| [Fast-packet transport](#nmea-2000-fast-packet-transport) | ❌ No | ❌ No | Multi-frame transport handling |
| [PGN 127245](#pgn-127245--rudder) | ✅ Yes | ✅ Yes | Rudder |
| [PGN 127250](#pgn-127250--vessel-heading) | ✅ Yes | ✅ Yes | Vessel Heading |
| [PGN 127257](#pgn-127257--attitude) | ✅ Yes | ✅ Yes | Attitude (yaw, pitch, roll) |
| [PGN 128259](#pgn-128259--speed-water-referenced) | ✅ Yes | ✅ Yes | Speed, Water Referenced |
| [PGN 130306](#pgn-130306--wind-data) | ✅ Yes | ✅ Yes | Wind Data |

---

# Detailed Message/PGN Descriptions

## NMEA 0183

This information has beed sourced from the [gpsd](https://gpsd.gitlab.io/gpsd/NMEA.html) documentation. All credits go to Eric S. Raymond <esr@thyrsus.com>.

### GGA — Global Positioning System Fix Data

Provides time, position, and fix-related data for a GPS receiver.

**Sentence format**

```text
                                                      11
        1         2       3 4        5 6 7  8   9  10 |  12 13  14   15
        |         |       | |        | | |  |   |   | |   | |   |    |
$--GGA,hhmmss.ss,ddmm.mm,a,dddmm.mm,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | UTC time (`hhmmss.ss`) | Hours, minutes, seconds |
| 2 | Latitude (`ddmm.mm`) | Degrees and minutes |
| 3 | Latitude hemisphere | `N` or `S` |
| 4 | Longitude (`dddmm.mm`) | Degrees and minutes |
| 5 | Longitude hemisphere | `E` or `W` |
| 6 | GPS quality indicator | `0` = Fix not available, `1` = GPS fix, `2` = DGPS fix, `3` = PPS fix, `4` = RTK, `5` = Float RTK, `6` = Estimated, `7` = Manual input mode, `8` = Simulation mode |
| 7 | Satellites in use | `00` to `12` |
| 8 | Horizontal dilution of precision (HDOP) | Meters |
| 9 | Antenna altitude | Above/below mean sea level (geoid), meters |
| 10 | Altitude units | `M` (meters) |
| 11 | Geoidal separation | Difference between WGS-84 ellipsoid and mean sea level (`-` means mean sea level is below ellipsoid) |
| 12 | Geoidal separation units | `M` (meters) |
| 13 | Age of DGPS data | Seconds since last SC104 Type 1 or 9 update (empty if unused) |
| 14 | Differential reference station ID | `0000` to `1023` |
| 15 | Checksum | `*hh` |

> The number of digits after the decimal point for time, latitude, and longitude is model-dependent.

**Example**

```text
$GNGGA,001043.00,4404.14036,N,12118.85961,W,1,12,0.98,1113.0,M,-21.3,M*47
```

> Note: Jackson Labs replaces the quality indicator with GPSDO status.


### GLL — Geographic Position - Latitude/Longitude

Provides latitude, longitude, and time of position fix.

**Sentence format**

```text
        1       2 3        4 5         6 7
        |       | |        | |         | |
$--GLL,ddmm.mm,a,dddmm.mm,a,hhmmss.ss,a*hh<CR><LF>
```
NMEA 2.3 and later:
```text
$--GLL,ddmm.mm,a,dddmm.mm,a,hhmmss.ss,a,m*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Latitude (`ddmm.mm`) | Degrees and minutes |
| 2 | Latitude hemisphere | `N` or `S` |
| 3 | Longitude (`dddmm.mm`) | Degrees and minutes |
| 4 | Longitude hemisphere | `E` or `W` |
| 5 | UTC time (`hhmmss.ss`) | Hours, minutes, seconds |
| 6 | Status | `A` = Data valid, `V` = Data invalid |
| 7 | FAA mode indicator | NMEA 2.3 and later |
| 8 | Checksum | `*hh` |

> The number of digits after the decimal point for time, latitude, and longitude is model-dependent.

**Example**

```text
$GNGLL,4404.14012,N,12118.85993,W,001037.00,A,A*67
```

### RMC — Recommended Minimum Navigation Information

Provides essential minimum navigation data.

**Sentence format**

```text
        1         2 3       4 5        6  7   8   9    10 11
        |         | |       | |        |  |   |   |    |  |
$--RMC,hhmmss.ss,A,ddmm.mm,a,dddmm.mm,a,x.x,x.x,xxxx,x.x,a*hh<CR><LF>
```
NMEA 2.3:
```text
$--RMC,hhmmss.ss,A,ddmm.mm,a,dddmm.mm,a,x.x,x.x,xxxx,x.x,a,m*hh<CR><LF>
```
NMEA 4.1:
```text
$--RMC,hhmmss.ss,A,ddmm.mm,a,dddmm.mm,a,x.x,x.x,xxxx,x.x,a,m,s*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | UTC time (`hhmmss.ss`) | Hours, minutes, seconds |
| 2 | Status | `A` = Valid, `V` = Warning |
| 3 | Latitude (`ddmm.mm`) | Degrees and minutes |
| 4 | Latitude hemisphere | `N` or `S` |
| 5 | Longitude (`dddmm.mm`) | Degrees and minutes |
| 6 | Longitude hemisphere | `E` or `W` |
| 7 | Speed over ground | Knots |
| 8 | Track made good | Degrees true |
| 9 | Date (`ddmmyy`) | UTC date |
| 10 | Magnetic variation | Degrees |
| 11 | Magnetic variation direction | `E` or `W` |
| 12 | FAA mode indicator | NMEA 2.3 and later |
| 13 | Navigation status | NMEA 4.1 and later: `A` Autonomous, `D` Differential, `E` Estimated, `M` Manual input mode, `N` Not valid, `S` Simulator, `V` Valid |
| 14 | Checksum | `*hh` |

> A status of `V` means the GPS has a valid fix that is below an internal quality threshold, e.g., because the dilution of precision is too high or an elevation mask test failed.

> The number of digits after the decimal point for time, latitude, and longitude is model-dependent.

**Example**

```text
$GNRMC,001031.00,A,4404.13993,N,12118.86023,W,0.146,,100117,,,A*7B
```

### VTG — Track Made Good and Ground Speed

Provides course over ground and ground speed information.

**Sentence format**

```text
         1  2  3  4  5  6  7  8 9
         |  |  |  |  |  |  |  | |
$--VTG,x.x,T,x.x,M,x.x,N,x.x,K*hh<CR><LF>
```
NMEA 2.3 and later:
```text
$--VTG,x.x,T,x.x,M,x.x,N,x.x,K,m*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Course over ground (degrees True) | Numeric angle |
| 2 | `T` | True reference indicator |
| 3 | Course over ground (degrees Magnetic) | Numeric angle |
| 4 | `M` | Magnetic reference indicator |
| 5 | Speed over ground (knots) | Numeric speed |
| 6 | `N` | Knots unit indicator |
| 7 | Speed over ground (km/h) | Numeric speed |
| 8 | `K` | Kilometers per hour unit indicator |
| 9 | FAA mode indicator | NMEA 2.3 and later |
| 10 | Checksum | `*hh` |

> In some older versions of NMEA 0183, the sentence appears as:

```text
         1  2  3   4  5
         |  |  |   |  |
$--VTG,x.x,x,x.x,x.x*hh<CR><LF>
```

**Old format field definitions**

| # | Field | Description |
|---|-------|-------------|
| 1 | True course over ground (degrees) | `000` to `359` |
| 2 | Magnetic course over ground (degrees) | `000` to `359` |
| 3 | Speed over ground (knots) | `00.0` to `99.9` |
| 4 | Speed over ground (kilometers) | `00.0` to `99.9` |
| 5 | Checksum | `*hh` |

> The two forms can be distinguished by field 2, which will be the fixed text `T` in the newer form. The new form appears to have been introduced with NMEA 3.01 in 2002.

> Some devices, such as those described in [GLOBALSAT], leave the magnetic-bearing fields 3 and 4 empty.

**Example**

```text
$GPVTG,220.86,T,,M,2.550,N,4.724,K,A*34
```

### VHW — Water Speed and Heading

Provides vessel heading and speed relative to the water.

**Sentence format**

```text
        1   2 3   4 5   6 7   8 9
        |   | |   | |   | |   | |
$--VHW,x.x,T,x.x,M,x.x,N,x.x,K*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Heading degrees (True) | Numeric angle |
| 2 | `T` | True reference indicator |
| 3 | Heading degrees (Magnetic) | Numeric angle |
| 4 | `M` | Magnetic reference indicator |
| 5 | Speed of vessel relative to water (knots) | Numeric speed |
| 6 | `N` | Knots unit indicator |
| 7 | Speed of vessel relative to water (km/h) | Numeric speed |
| 8 | `K` | Kilometers per hour unit indicator |
| 9 | Checksum | `*hh` |

> [GLOBALSAT] describes a different format in which the first three fields are water-temperature measurements. It’s not clear which is correct.

### HDG — Heading, Deviation & Variation

## Sentence Structure

```text
        1   2   3 4   5 6
        |   |   | |   | |
 $--HDG,x.x,x.x,a,x.x,a*hh<CR><LF>
```

#### Field definitions

| # | Field | Unit | Description |
|---|-------|------|-------------|
| 1 | Magnetic sensor heading | degrees | Heading as reported by magnetic sensor |
| 2 | Magnetic deviation | degrees | Magnitude of deviation |
| 3 | Deviation direction | — | `E` = Easterly, `W` = Westerly |
| 4 | Magnetic variation | degrees | Magnitude of variation |
| 5 | Variation direction | — | `E` = Easterly, `W` = Westerly |
| 6 | Checksum | — | `*hh` |

### DBT — Depth Below Transducer
 
Reports water depth below the transducer in multiple units.

**Sentence format**

```text
        1   2 3   4 5   6 7
        |   | |   | |   | |
$--DBT,x.x,f,x.x,M,x.x,F*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Water depth (feet) | Numeric depth |
| 2 | `f` | Feet unit indicator |
| 3 | Water depth (meters) | Numeric depth |
| 4 | `M` | Meters unit indicator |
| 5 | Water depth (fathoms) | Numeric depth |
| 6 | `F` | Fathoms unit indicator |
| 7 | Checksum | `*hh` |

> In real-world sensors, sometimes not all three conversions are reported.  
> For example: `$SDDBT,,f,22.5,M,,F*cs`

**Example**

```text
$SDDBT,7.8,f,2.4,M,1.3,F*0D
```

### DPT — Depth of Water

## Sentence Structure
```
        1   2   3   4
        |   |   |   |
 $--DPT,x.x,x.x,x.x*hh<CR><LF>
```

#### Field definitions

| # | Field | Unit | Description |
|---|-------|------|-------------|
| 1 | Water depth | meters | Depth relative to transducer |
| 2 | Transducer offset | meters | Positive = distance from transducer to waterline; Negative = distance from transducer to keel |
| 3 | Max range scale | meters | Maximum range scale in use *(NMEA 3.0 and above)* |
| 4 | Checksum | — | `*hh` |

## Example
```
$INDPT,2.3,0.0*46
```

## Notes

> ⚠️ This sentence was incorrectly titled **"Heading - Deviation & Variation"** in [BETKE].
>
> Reference: [Humminbird DPT Documentation](http://www.humminbird.com/normal.asp?id=853)

### MWV — Wind Speed and Angle
  
Reports wind angle and speed, with reference and validity.

**Sentence format**

```text
        1   2 3   4 5
        |   | |   | |
$--MWV,x.x,a,x.x,a*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Wind angle | `0` to `359` degrees |
| 2 | Reference | `R` = Relative, `T` = True |
| 3 | Wind speed | Numeric value |
| 4 | Wind speed units | `K` = km/h, `M` = m/s, `N` = knots |
| 5 | Status | `A` = Data valid, `V` = Invalid |
| 6 | Checksum | `*hh` |

**Example**

```text
$WIMWV,45.0,R,10.5,N,A*1F
```

### GSA — GPS DOP and Active Satellites

Reports DOP (Dilution of Precision) and active satellites used for the fix.

**Sentence format**

```text
        1 2 3                        14 15  16  17  18
        | | |                         |  |   |   |   |
$--GSA,a,a,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x.x,x.x,x.x*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Selection mode | `M` = Manual (forced 2D/3D), `A` = Automatic (2D/3D) |
| 2 | Mode | `1` = No fix, `2` = 2D fix, `3` = 3D fix |
| 3 | ID of 1st satellite used for fix | Satellite ID |
| 4 | ID of 2nd satellite used for fix | Satellite ID |
| 5 | ID of 3rd satellite used for fix | Satellite ID |
| 6 | ID of 4th satellite used for fix | Satellite ID |
| 7 | ID of 5th satellite used for fix | Satellite ID |
| 8 | ID of 6th satellite used for fix | Satellite ID |
| 9 | ID of 7th satellite used for fix | Satellite ID |
| 10 | ID of 8th satellite used for fix | Satellite ID |
| 11 | ID of 9th satellite used for fix | Satellite ID |
| 12 | ID of 10th satellite used for fix | Satellite ID |
| 13 | ID of 11th satellite used for fix | Satellite ID |
| 14 | ID of 12th satellite used for fix | Satellite ID |
| 15 | PDOP | Position Dilution of Precision |
| 16 | HDOP | Horizontal Dilution of Precision |
| 17 | VDOP | Vertical Dilution of Precision |
| 18 | System ID | NMEA 4.11+, optional, just before checksum: `1` GPS L1C/A L2CL L2CM, `2` GLONASS L1 OF L2 OF, `3` Galileo E1C E1B E5 bl E5 bQ, `4` BeiDou B1I D1 B1I D2 B2I D1 B2I D12 |
| 19 | Checksum | `*hh` |

> NMEA 4.1+ systems (e.g., u-blox 9, Quectel LCD79) may emit the extra System ID field before the checksum.

**Example**

```text
$GNGSA,A,3,80,71,73,79,69,,,,,,,,1.83,1.09,1.47*17
```

### MTW — Mean Temperature of Water

Reports the mean water temperature.

**Sentence format**

```text
        1   2 3
        |   | |
$--MTW,x.x,C*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Temperature | Degrees |
| 2 | Unit of measurement | `C` = Celsius |
| 3 | Checksum | `*hh` |

> [GLOBALSAT] lists this as "Meteorological Temperature of Water", which is probably incorrect.

**Example**

```text
$INMTW,17.9,C*1B
```

### VWR — Relative Wind Speed and Angle

Reports relative wind direction and speed in multiple units.

**Sentence format**

```text
         1  2  3  4  5  6  7  8 9
         |  |  |  |  |  |  |  | |
$--VWR,x.x,a,x.x,N,x.x,M,x.x,K*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Wind direction magnitude | Degrees |
| 2 | Wind direction | `L` = Left of bow, `R` = Right of bow |
| 3 | Speed (knots) | Numeric speed |
| 4 | `N` | Knots unit indicator |
| 5 | Speed (meters per second) | Numeric speed |
| 6 | `M` | Meters per second unit indicator |
| 7 | Speed (kilometers per hour) | Numeric speed |
| 8 | `K` | Kilometers per hour unit indicator |
| 9 | Checksum | `*hh` |

### ZDA — Time & Date (UTC, Day, Month, Year, Local Time Zone)

Reports UTC time, date, and local time zone offset.

**Sentence format**

```text
        1         2  3  4    5  6  7
        |         |  |  |    |  |  |
$--ZDA,hhmmss.ss,xx,xx,xxxx,xx,xx*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | UTC time | Hours, minutes, seconds (may have fractional subseconds) |
| 2 | Day | `01` to `31` |
| 3 | Month | `01` to `12` |
| 4 | Year | Four digits |
| 5 | Local zone hours | `00` to `±13` (hours offset from UTC) |
| 6 | Local zone minutes | `00` to `59` (apply same sign as local hours) |
| 7 | Checksum | `*hh` |

**Example**

```text
$GPZDA,160012.71,11,03,2004,-1,00*7D
```

### APB — Autopilot Sentence "B"

This is a fixed form of the **APA** sentence with some ambiguities removed.

> **Note:** Some autopilots (Robertson in particular) misinterpret **"bearing from origin to destination"** as **"bearing from present position to destination"**.  
> This is likely due to the difference between the **APB** and **APA** sentences. For the **APA** sentence this interpretation would be correct for the data in the same field.  
> APA differs from APB only in this field and also omits the last two fields where this distinction is clearly defined.  
> This may result in poor performance if the vessel is sufficiently off-course and the two bearings differ.

**Sentence format**

```text
                  1 2 3   4 5 6 7 8   9 10   11  12|   14|
                  | | |   | | | | |   | |    |   | |   | |
$--APB,A,A,x.x,a,N,A,A,x.x,a,c--c,x.x,a,x.x,a*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Status | `A` = Data valid, `V` = Loran-C Blink/SNR warning or general warning when reliable fix is unavailable |
| 2 | Status | `V` = Loran-C Cycle Lock warning flag, `A` = OK or not used |
| 3 | Cross Track Error Magnitude | Cross-track error value |
| 4 | Direction to steer | `L` = Left, `R` = Right |
| 5 | Cross Track Units | `N` = Nautical miles |
| 6 | Arrival circle status | `A` = Arrival circle entered |
| 7 | Perpendicular passed at waypoint | `A` = Perpendicular passed |
| 8 | Bearing from origin to destination | Numeric bearing |
| 9 | Bearing type | `M` = Magnetic, `T` = True |
| 10 | Destination waypoint ID | Waypoint identifier |
| 11 | Bearing from present position to destination | Numeric bearing |
| 12 | Bearing type | `M` = Magnetic, `T` = True |
| 13 | Heading to steer to destination waypoint | Numeric heading |
| 14 | Heading type | `M` = Magnetic, `T` = True |
| 15 | Checksum | `*hh` |

**Example**

```text
$GPAPB,A,A,0.10,R,N,V,V,011,M,DEST,011,M,011,M*82
```

### RMB — Recommended Minimum Navigation Information

Sent by a navigation receiver when a **destination waypoint is active**.

**Sentence format**

```text
                                                                                                                                                                 14
                  1 2   3 4    5    6       7 8        9 10  11  12  13|
                  | |   | |    |    |       | |        | |   |   |   | |
$--RMB,A,x.x,a,c--c,c--c,llll.ll,a,yyyyy.yy,a,x.x,x.x,x.x,A*hh<CR><LF>
```

**NMEA 2.3 and later**

```text
$--RMB,A,x.x,a,c--c,c--c,llll.ll,a,yyyyy.yy,a,x.x,x.x,x.x,A,m*hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Status | `A` = Active, `V` = Invalid |
| 2 | Cross Track Error | Nautical miles |
| 3 | Direction to steer | `L` = Left, `R` = Right |
| 4 | Origin waypoint ID | Waypoint identifier |
| 5 | Destination waypoint ID | Waypoint identifier |
| 6 | Destination waypoint latitude (`llll.ll`) | Latitude value |
| 7 | Latitude hemisphere | `N` = North, `S` = South |
| 8 | Destination waypoint longitude (`yyyyy.yy`) | Longitude value |
| 9 | Longitude hemisphere | `E` = East, `W` = West |
| 10 | Range to destination | Nautical miles |
| 11 | Bearing to destination | Degrees true |
| 12 | Destination closing velocity | Knots |
| 13 | Arrival status | `A` = Arrival circle entered, `V` = Not entered/passed |
| 14 | FAA mode indicator | Present in NMEA 2.3 and later |
| 15 | Checksum | `*hh` |

**Example**

```text
$GPRMB,A,0.66,L,003,004,4917.24,N,12309.57,W,001.3,052.5,000.5,V*0B
```

### XDR — Transducer Measurement

```text
                  1 2   3 4            n
                  | |   | |            |
 $--XDR,a,x.x,a,c--c, ..... *hh<CR><LF>
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Transducer type | Single character identifier |
| 2 | Measurement data | Numeric value |
| 3 | Units | See table below |
| 4 | Transducer name | Free-form label |

Fields 1–4 form a **quadruplet** and may repeat any number of times (`n`), each describing one sensor. The message ends with a standard checksum.

#### Units of measurement

| Code | Unit |
|------|------|
| `A` | Amperes |
| `B` | Bars / Binary |
| `C` | Celsius |
| `D` | Degrees |
| `H` | Hertz |
| `I` | Litres/second |
| `K` | Kelvin / kg/m³ |
| `M` | Metres / Cubic metres |
| `N` | Newton |
| `P` | % of full range / Pascal |
| `R` | RPM |
| `S` | Parts per thousand |
| `V` | Volts |

#### Example

```text
$HCXDR,A,171,D,PITCH,A,-37,D,ROLL,G,367,,MAGX,G,2420,,MAGY,G,-8984,,MAGZ*41
```

This sentence contains five quadruplets: pitch, roll, and three magnetic field components (X, Y, Z).

### HDM — Heading, Magnetic

Vessel heading in degrees with respect to magnetic north produced by any device or system producing magnetic heading.

```text
$--HDM,x.x,M*hh
```

#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Heading degrees | Magnetic heading value |
| 2 | `M` | Magnetic reference indicator |
| 3 | Checksum | `*hh` |

### XTE — Cross-Track Error, Measured

XTE Sentencies are formatted as:

```text
 $--XTE,A,A,x.x,a,N*hh<CR><LF>
```
or, in NMEA 2.3 standard:
```text
 $--XTE,A,A,x.x,a,N,m*hh<CR><LF>
```
#### Field definitions

| # | Field | Description |
|---|-------|-------------|
| 1 | Status | `A` = Valid, `V` = Loran-C Blink or SNR warning, or general warning when reliable fix is unavailable |
| 2 | Status | `V` = Loran-C Cycle Lock warning flag, `A` = Valid |
| 3 | Cross Track Error Magnitude | Cross-track error value |
| 4 | Direction to steer | `L` or `R` |
| 5 | Cross Track Units | `N` = Nautical miles |
| 6 | FAA mode indicator | NMEA 2.3 and later, optional |
| 7 | Checksum | `*hh` |

Example: `$GPXTE,V,V,,,N,S*43`

### VLW — Distance Traveled through Water

## Sentence Structure
```
                  1   2 3   4 5   6  7  8  9
                  |   | |   | |   |  |  |  |
 $--VLW,x.x,N,x.x,N,x.x,N,x.x,N*hh<CR><LF>
```

#### Field definitions

| # | Field | Unit | Description |
|---|-------|------|-------------|
| 1 | Total cumulative water distance | nm | Total distance through water since manufacture/reset |
| 2 | Unit | — | `N` = Nautical Miles |
| 3 | Water distance since reset | nm | Distance through water since last reset |
| 4 | Unit | — | `N` = Nautical Miles |
| 5 | Total cumulative ground distance | nm | *(NMEA 3.0 and above)* |
| 6 | Unit | — | `N` = Nautical Miles *(NMEA 3.0 and above)* |
| 7 | Ground distance since reset | nm | *(NMEA 3.0 and above)* |
| 8 | Unit | — | `N` = Nautical Miles *(NMEA 3.0 and above)* |
| 9 | Checksum | — | `*hh` |

### RMA — Recommended Minimum Navigation Information

## Sentence Structure
```
                                                                                                                                         12
                  1 2       3 4        5 6   7   8   9   10  11|
                  | |       | |        | |   |   |   |   |   | |
 $--RMA,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,x.x,x.x,x.x,a*hh<CR><LF>
```

#### Field definitions

| # | Field | Unit | Description |
|---|-------|------|-------------|
| 1 | Status | — | `A` = Valid, `V` = Invalid |
| 2 | Latitude | — | `llll.ll` format |
| 3 | Latitude direction | — | `N` = North, `S` = South |
| 4 | Longitude | — | `yyyyy.yy` format |
| 5 | Longitude direction | — | `E` = East, `W` = West |
| 6 | Time Difference A | μs | Loran-C time difference A |
| 7 | Time Difference B | μs | Loran-C time difference B |
| 8 | Speed over ground | knots | — |
| 9 | Track made good | degrees | True heading of travel |
| 10 | Magnetic variation | degrees | — |
| 11 | Variation direction | — | `E` = Easterly, `W` = Westerly |
| 12 | Checksum | — | `*hh` |

### GSV — Satellites in View

> One of the sentences commonly emitted by GPS units. Describes the sky position of GPS satellites currently in view. Typically transmitted in a group of 2 or 3 sentences.

## Sentence Structure
```
                  1 2 3 4 5 6 7     n
                  | | | | | | |     |
 $--GSV,x,x,x,x,x,x,x,...*hh<CR><LF>
```

#### Field definitions

| # | Field | Range | Description |
|---|-------|-------|-------------|
| 1 | Total sentences | — | Total number of GSV sentences in this group |
| 2 | Sentence number | 1–9 | Index of this sentence within the group |
| 3 | Satellites in view | — | Total satellites in view (leading zeros sent) |
| 4 | Satellite ID / PRN | — | Satellite ID or PRN number (leading zeros sent) |
| 5 | Elevation | -90 to 90° | Elevation in degrees (leading zeros sent) |
| 6 | Azimuth | 000 to 359° | Azimuth to true north in degrees (leading zeros sent) |
| 7 | SNR | 00–99 dB | Signal-to-noise ratio (leading zeros sent; null if not tracking) |
| … | *(repeat 4–7)* | — | Additional satellite quadruples (fields 4–7 repeated per satellite) |
| n-1 | Signal ID | — | *(NMEA 4.11 and above)* |
| n | Checksum | — | `*hh` |

## Examples
```
$GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74
$GPGSV,3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00*74
$GPGSV,3,3,11,22,42,067,42,24,14,311,43,27,05,244,00,,,,*4D
```
```
$GLGSV,3,3,09,88,07,028*51
```

## Notes

> ⚠️ **PRN vs Satellite ID:** `$GNGSV` uses PRN in field 4, while other `$GxGSV` variants use Satellite ID. Several manufacturers (Jackson Labs, Quectel, Telit, and others) implement this inconsistently.

> ℹ️ **Extra quadruples:** Some receivers emit more than 12 satellite quadruples (more than three sentences), even though NMEA-0183 does not formally allow this — WAAS satellites are a common example.

> ℹ️ **Untracked satellites:** Receivers may report quadruples for satellites they are not actively tracking, in which case the SNR field will be null.

> ℹ️ **Signal ID (NMEA 4.10+):** Systems such as u-blox 9 and Quectel LC79 may include an extra Signal ID field immediately before the checksum.

## NMEA 2000

### PGN 127245 — Rudder

Reports rudder command/feedback values for a specific rudder channel.

#### Field definitions

1. **Rudder ID** — rudder instance identifier
2. **Direction** — rudder direction selector (`0=none`, `1=starboard`, `2=port`)
3. **Angle order** — signed radians, resolution 1e-4
4. **Position** — signed radians, resolution 1e-4
5. **Reserved** — protocol-reserved bits/bytes for future use

### PGN 127250 — Vessel Heading

Reports vessel heading and related magnetic correction values.

#### Field definitions

1. **Sequence ID** — message sequence counter  
2. **Heading** — radians, resolution 1e-4  
3. **Deviation** — signed radians, resolution 1e-4  
4. **Variation** — signed radians, resolution 1e-4  
5. **Heading reference** — true or magnetic reference selector

### PGN 127257 — Attitude

Reports vessel attitude in yaw, pitch, and roll.

#### Field definitions

1. **Sequence ID** — message sequence counter  
2. **Yaw** — signed radians, resolution 1e-4  
3. **Pitch** — signed radians, resolution 1e-4  
4. **Roll** — signed radians, resolution 1e-4  
5. **Reserved** — reserved byte for future use

### PGN 128259 — Speed, Water Referenced

Reports water-referenced and ground-referenced vessel speed.

#### Field definitions

1. **Sequence ID** — message sequence counter  
2. **Speed water referenced** — m/s, resolution 1e-2  
3. **Speed ground referenced** — m/s, resolution 1e-2  
4. **Speed water referenced type** — sensor/source indicator  
5. **Speed direction** — direction relative to the water

### PGN 130306 — Wind Data

Reports wind speed and wind direction with reference type.

#### Field definitions

1. **Sequence ID** — message sequence counter  
2. **Wind speed** — m/s, resolution 1e-2  
3. **Wind direction** — radians, resolution 1e-4  
4. **Wind reference** — true/apparent wind reference selector
