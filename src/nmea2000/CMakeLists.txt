# Single-message NMEA 2000 targets
# add_library(PGN128259 STATIC PGN128259.cpp)
# target_include_directories(PGN128259 PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Monolithic NMEA 2000 target (contains all nmea2000 infrastructure)
add_library(nmealib2000 STATIC
    ../nmea2000.cpp
    messageRegistry.cpp
    nmea2000Factory.cpp
    PGN128259.cpp
    PGN127245.cpp
    PGN127250.cpp
    PGN127251.cpp
    PGN127257.cpp
    PGN129025.cpp
    PGN129026.cpp
    PGN130306.cpp
)

target_include_directories(nmealib2000 PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Expose NMEA 2000 sources to parent for nmealib composite target
set(NMEALIB_NMEA2000_SOURCES
    nmea2000.cpp
    nmea2000/messageRegistry.cpp
    nmea2000/nmea2000Factory.cpp
    nmea2000/PGN128259.cpp
    nmea2000/PGN127245.cpp
    nmea2000/PGN127250.cpp
    nmea2000/PGN127251.cpp
    nmea2000/PGN127257.cpp
    nmea2000/PGN129025.cpp
    nmea2000/PGN129026.cpp
    nmea2000/PGN130306.cpp
    PARENT_SCOPE
)
