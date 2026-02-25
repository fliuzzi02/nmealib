add_subdirectory(nmea0183)

add_library(nmealib_core STATIC
    message.cpp
    nmeaException.cpp
)

target_include_directories(nmealib_core PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS nmealib_core
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    COMPONENT library
)

# Public umbrella target for downstream consumers (header + component libraries)
add_library(nmealib INTERFACE)

target_link_libraries(nmealib INTERFACE 
    nmealib_nmea0183
)

target_include_directories(nmealib INTERFACE 
    ${PROJECT_SOURCE_DIR}/include
)
