# APB API

Class `nmea0183.APB` (Python) / `nmealib::nmea0183::APB` (C++).

## Creation
- Python direct: 
```python
msg = nmealib.nmea0183.APB(<fields...>)
```
- C++ direct:
```cpp
nmealib::nmea0183::APB msg(/* fields */);
```
- Python factory: 
```python
msg = nmealib.nmea0183.Nmea0183Factory.create(raw_sentence)
```
- C++ factory: 
```cpp
auto msg = nmealib::nmea0183::Nmea0183Factory::create(rawSentence);
```

## Public Methods

### `get_arrival_circle_status` / `getArrivalCircleStatus`

```cpp
// CPP signature here (return type, parameters)
```

Description of what this method does. And the meaning of what it returns.

## `get_bearing_origin_to_destination` / `getBearingOriginToDestination`
```cpp
// CPP signature here (return type, parameters)
```
Description of what this method does. And the meaning of what it returns.

... all the other getters in the same format ...

## Notes
- Inherits `Message0183` and `Message` APIs (`serialize`, `validate`, `clone`, raw/timestamp/type getters).
