add_library(nmealib_nmea0183 STATIC
    nmea0183.cpp
)

# Public headers live under include/ at the repository root
target_include_directories(nmealib_nmea0183 PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(nmealib_nmea0183 PUBLIC cxx_std_20)