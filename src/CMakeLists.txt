add_subdirectory(nmea0183)
add_subdirectory(nmea2000)

# Monolithic nmealib target (contains everything)
add_library(nmealib STATIC
    message.cpp
    nmeaException.cpp
    ${NMEALIB_NMEA0183_SOURCES}
    ${NMEALIB_NMEA2000_SOURCES}
)

target_include_directories(nmealib PUBLIC ${PROJECT_SOURCE_DIR}/include)

install(TARGETS nmealib
    ARCHIVE DESTINATION lib COMPONENT library
    LIBRARY DESTINATION lib COMPONENT library
)
