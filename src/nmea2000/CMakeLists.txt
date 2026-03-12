# Single-message NMEA 2000 targets
add_library(PGN128259 STATIC PGN128259.cpp)
target_include_directories(PGN128259 PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(PGN127250 STATIC PGN127250.cpp)
target_include_directories(PGN127250 PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(PGN130306 STATIC PGN130306.cpp)
target_include_directories(PGN130306 PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Monolithic NMEA 2000 target (contains all nmea2000 infrastructure)
add_library(nmealib2000 STATIC
    ../nmea2000.cpp
    messageRegistry.cpp
    nmea2000Factory.cpp
    PGN128259.cpp
    PGN127250.cpp
    PGN130306.cpp
)

target_include_directories(nmealib2000 PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Expose NMEA 2000 sources to parent for nmealib composite target
set(NMEALIB_NMEA2000_SOURCES
    nmea2000.cpp
    nmea2000/messageRegistry.cpp
    nmea2000/nmea2000Factory.cpp
    nmea2000/PGN128259.cpp
    nmea2000/PGN127250.cpp
    nmea2000/PGN130306.cpp
    PARENT_SCOPE
)
