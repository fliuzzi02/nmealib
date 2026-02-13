add_subdirectory(nmea0183)

# Public umbrella target for downstream consumers (header + component libraries)
add_library(nmealib INTERFACE)
target_link_libraries(nmealib INTERFACE 
    nmealib_nmea0183
)

target_include_directories(nmealib INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)