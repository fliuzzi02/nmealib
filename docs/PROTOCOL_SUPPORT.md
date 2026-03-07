# Protocol Support Matrix

This page tracks message/PGN support and implementation details.

## NMEA 0183

| Message | Implemented | Tested| Notes |
|---|---|---|---|
| [DBT](#dbt--depth-below-transducer) | ✅ Yes | ✅ Yes | Depth Below Transducer |
| [GGA](#gga--global-positioning-system-fix-data) | ✅ Yes | ✅ Yes | Global Positioning System Fix Data |
| [GLL](#gll--geographic-position---latitudelongitude) | ✅ Yes | ✅ Yes | Geographic Position |
| [GSA](#gsa--gps-dop-and-active-satellites) | ✅ Yes | ✅ Yes | GPS DOP and Active Satellites |
| [MTW](#mtw--mean-temperature-of-water) | ✅ Yes | ✅ Yes | Mean Temperature of Water |
| [MWV](#mwv--wind-speed-and-angle) | ✅ Yes | ✅ Yes | Wind Speed and Angle |
| [RMC](#rmc--recommended-minimum-navigation-information) | ✅ Yes | ✅ Yes | Recommended Minimum Navigation Data |
| [VHW](#vhw--water-speed-and-heading) | ✅ Yes | ✅ Yes | Water Speed and Heading |
| [VTG](#vtg--track-made-good-and-ground-speed) | ✅ Yes | ✅ Yes | Course Over Ground and Ground Speed |
| [VWR](#vwr--relative-wind-speed-and-angle) | ✅ Yes | ✅ Yes | Relative Wind Speed and Angle |
| [ZDA](#zda--time--date-utc-day-month-year-local-time-zone) | ✅ Yes | ✅ Yes | Time and Date |

## NMEA 2000

| Message / Transport | Implemented | Tested | Notes |
|---|---|---|---|
| [Single-frame messages](#nmea-2000-single-frame-messages) | ✅ Yes | ❌ No | Standard one-frame CAN payloads |
| [Fast-packet transport](#nmea-2000-fast-packet-transport) | ✅ Yes | ❌ No | Multi-frame transport handling |

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

1. **UTC time** (`hhmmss.ss`) — hours, minutes, seconds  
2. **Latitude** (`ddmm.mm`) — degrees and minutes  
3. **Latitude hemisphere** — `N` or `S`  
4. **Longitude** (`dddmm.mm`) — degrees and minutes  
5. **Longitude hemisphere** — `E` or `W`  
6. **GPS quality indicator** (non-null):
   - `0` = Fix not available
   - `1` = GPS fix
   - `2` = Differential GPS (DGPS) fix
   - `3` = PPS fix
   - `4` = Real Time Kinematic (RTK)
   - `5` = Float RTK
   - `6` = Estimated (dead reckoning)
   - `7` = Manual input mode
   - `8` = Simulation mode
7. **Satellites in use** — `00` to `12`  
8. **Horizontal dilution of precision (HDOP)** — meters  
9. **Antenna altitude** above/below mean sea level (geoid) — meters  
10. **Altitude units** — `M` (meters)  
11. **Geoidal separation** — difference between WGS-84 ellipsoid and mean sea level  
    (`-` means mean sea level is below ellipsoid)
12. **Geoidal separation units** — `M` (meters)  
13. **Age of DGPS data** — seconds since last SC104 Type 1 or 9 update (empty if unused)  
14. **Differential reference station ID** — `0000` to `1023`  
15. **Checksum** — `*hh`

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

1. **Latitude** (`ddmm.mm`) — degrees and minutes  
2. **Latitude hemisphere** — `N` or `S`  
3. **Longitude** (`dddmm.mm`) — degrees and minutes  
4. **Longitude hemisphere** — `E` or `W`  
5. **UTC time** (`hhmmss.ss`) — hours, minutes, seconds  
6. **Status** — `A` = Data valid, `V` = Data invalid  
7. **FAA mode indicator** (NMEA 2.3 and later)  
8. **Checksum** — `*hh`

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

1. **UTC time** (`hhmmss.ss`) — hours, minutes, seconds  
2. **Status** — `A` = Valid, `V` = Warning  
3. **Latitude** (`ddmm.mm`) — degrees and minutes  
4. **Latitude hemisphere** — `N` or `S`  
5. **Longitude** (`dddmm.mm`) — degrees and minutes  
6. **Longitude hemisphere** — `E` or `W`  
7. **Speed over ground** — knots  
8. **Track made good** — degrees true  
9. **Date** (`ddmmyy`)  
10. **Magnetic variation** — degrees  
11. **Magnetic variation direction** — `E` or `W`  
12. **FAA mode indicator** (NMEA 2.3 and later)  
13. **Navigation status** (NMEA 4.1 and later):  
    - `A` = Autonomous  
    - `D` = Differential  
    - `E` = Estimated  
    - `M` = Manual input mode  
    - `N` = Not valid  
    - `S` = Simulator  
    - `V` = Valid  
14. **Checksum** — `*hh`

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

1. **Course over ground (degrees True)**  
2. **T** — True  
3. **Course over ground (degrees Magnetic)**  
4. **M** — Magnetic  
5. **Speed over ground (knots)**  
6. **N** — Knots  
7. **Speed over ground (km/h)**  
8. **K** — Kilometers per hour  
9. **FAA mode indicator** (NMEA 2.3 and later)  
10. **Checksum** — `*hh`

> In some older versions of NMEA 0183, the sentence appears as:

```text
         1  2  3   4  5
         |  |  |   |  |
$--VTG,x.x,x,x.x,x.x*hh<CR><LF>
```

**Old format field definitions**

1. **True course over ground (degrees)** — 000 to 359  
2. **Magnetic course over ground (degrees)** — 000 to 359  
3. **Speed over ground (knots)** — 00.0 to 99.9  
4. **Speed over ground (kilometers)** — 00.0 to 99.9  
5. **Checksum** — `*hh`

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

1. **Heading degrees (True)**  
2. **T** — True  
3. **Heading degrees (Magnetic)**  
4. **M** — Magnetic  
5. **Speed of vessel relative to the water (knots)**  
6. **N** — Knots  
7. **Speed of vessel relative to the water (km/h)**  
8. **K** — Kilometers  
9. **Checksum** — `*hh`

> [GLOBALSAT] describes a different format in which the first three fields are water-temperature measurements. It’s not clear which is correct.

### DBT — Depth Below Transducer
 
Reports water depth below the transducer in multiple units.

**Sentence format**

```text
        1   2 3   4 5   6 7
        |   | |   | |   | |
$--DBT,x.x,f,x.x,M,x.x,F*hh<CR><LF>
```

#### Field definitions

1. **Water depth (feet)**  
2. **f** — feet  
3. **Water depth (meters)**  
4. **M** — meters  
5. **Water depth (fathoms)**  
6. **F** — fathoms  
7. **Checksum** — `*hh`

> In real-world sensors, sometimes not all three conversions are reported.  
> For example: `$SDDBT,,f,22.5,M,,F*cs`

**Example**

```text
$SDDBT,7.8,f,2.4,M,1.3,F*0D
```

### MWV — Wind Speed and Angle
  
Reports wind angle and speed, with reference and validity.

**Sentence format**

```text
        1   2 3   4 5
        |   | |   | |
$--MWV,x.x,a,x.x,a*hh<CR><LF>
```

#### Field definitions

1. **Wind angle** — 0 to 359 degrees  
2. **Reference** — `R` = Relative, `T` = True  
3. **Wind speed**  
4. **Wind speed units** — `K` = km/h, `M` = m/s, `N` = knots  
5. **Status** — `A` = Data valid, `V` = Invalid  
6. **Checksum** — `*hh`

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

1. **Selection mode** — `M` = Manual (forced 2D/3D), `A` = Automatic (2D/3D)  
2. **Mode** — `1` = No fix, `2` = 2D fix, `3` = 3D fix  
3. **ID of 1st satellite used for fix**  
4. **ID of 2nd satellite used for fix**  
5. **ID of 3rd satellite used for fix**  
6. **ID of 4th satellite used for fix**  
7. **ID of 5th satellite used for fix**  
8. **ID of 6th satellite used for fix**  
9. **ID of 7th satellite used for fix**  
10. **ID of 8th satellite used for fix**  
11. **ID of 9th satellite used for fix**  
12. **ID of 10th satellite used for fix**  
13. **ID of 11th satellite used for fix**  
14. **ID of 12th satellite used for fix**  
15. **PDOP** (Position Dilution of Precision)  
16. **HDOP** (Horizontal Dilution of Precision)  
17. **VDOP** (Vertical Dilution of Precision)  
18. **System ID** (NMEA 4.11+, optional, just before checksum):  
    - `1` = GPS L1C/A, L2CL, L2CM  
    - `2` = GLONASS L1 OF, L2 OF  
    - `3` = Galileo E1C, E1B, E5 bl, E5 bQ  
    - `4` = BeiDou B1I D1, B1I D2, B2I D1, B2I D12  
19. **Checksum** — `*hh`

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

1. **Temperature** — degrees  
2. **Unit of measurement** — `C` = Celsius  
3. **Checksum** — `*hh`

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

1. **Wind direction magnitude** — degrees  
2. **Wind direction** — `L` = Left of bow, `R` = Right of bow  
3. **Speed (knots)**  
4. **N** — Knots  
5. **Speed (meters per second)**  
6. **M** — Meters per second  
7. **Speed (kilometers per hour)**  
8. **K** — Kilometers per hour  
9. **Checksum** — `*hh`

### ZDA — Time & Date (UTC, Day, Month, Year, Local Time Zone)

Reports UTC time, date, and local time zone offset.

**Sentence format**

```text
        1         2  3  4    5  6  7
        |         |  |  |    |  |  |
$--ZDA,hhmmss.ss,xx,xx,xxxx,xx,xx*hh<CR><LF>
```

#### Field definitions

1. **UTC time** — hours, minutes, seconds (may have fractional subseconds)  
2. **Day** — `01` to `31`  
3. **Month** — `01` to `12`  
4. **Year** — 4 digits  
5. **Local zone hours** — `00` to `±13` (hours offset from UTC)  
6. **Local zone minutes** — `00` to `59` (apply same sign as local hours)  
7. **Checksum** — `*hh`

**Example**

```text
$GPZDA,160012.71,11,03,2004,-1,00*7D
```