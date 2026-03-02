# Monolithic NMEA 2000 target (contains all nmea2000 infrastructure)
add_library(nmealib2000 STATIC
    ../nmea2000.cpp
    messageRegistry.cpp
    nmea2000Factory.cpp
)

target_include_directories(nmealib2000 PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Expose NMEA 2000 sources to parent for nmealib composite target
set(NMEALIB_NMEA2000_SOURCES
    nmea2000.cpp
    nmea2000/messageRegistry.cpp
    nmea2000/nmea2000Factory.cpp
    PARENT_SCOPE
)

install(TARGETS
    nmealib2000
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    COMPONENT library
)
