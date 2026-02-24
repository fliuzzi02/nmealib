add_library(nmealib_nmea0183 STATIC
    ../message.cpp
    ../nmeaException.cpp
    nmea0183.cpp
    nmea0183Factory.cpp
    messageRegistry.cpp
    gga.cpp
    gsa.cpp
    gll.cpp
    mwv.cpp
    rmc.cpp
    vtg.cpp
    zda.cpp
)

# Public headers live under include/ at the repository root
target_include_directories(nmealib_nmea0183 PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(nmealib_nmea0183 PUBLIC cxx_std_20)
