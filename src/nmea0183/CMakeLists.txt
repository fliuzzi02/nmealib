add_library(nmealib_nmea0183 STATIC
    nmea0183.cpp
    nmea0183Factory.cpp
    rmc.cpp
)

# headers live in src/ and src/nmea0183/
target_include_directories(nmealib_nmea0183 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(nmealib_nmea0183 PUBLIC cxx_std_20)