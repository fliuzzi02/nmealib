# Single-message NMEA 0183 targets
add_library(nmealib0183_dbt STATIC dbt.cpp)
target_include_directories(nmealib0183_dbt PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(nmealib0183_gga STATIC gga.cpp)
target_include_directories(nmealib0183_gga PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(nmealib0183_gll STATIC gll.cpp)
target_include_directories(nmealib0183_gll PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(nmealib0183_gsa STATIC gsa.cpp)
target_include_directories(nmealib0183_gsa PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(nmealib0183_mtw STATIC mtw.cpp)
target_include_directories(nmealib0183_mtw PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(nmealib0183_mwv STATIC mwv.cpp)
target_include_directories(nmealib0183_mwv PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(nmealib0183_rmc STATIC rmc.cpp)
target_include_directories(nmealib0183_rmc PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(nmealib0183_vhw STATIC vhw.cpp)
target_include_directories(nmealib0183_vhw PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(nmealib0183_vtg STATIC vtg.cpp)
target_include_directories(nmealib0183_vtg PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(nmealib0183_zda STATIC zda.cpp)
target_include_directories(nmealib0183_zda PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Monolithic NMEA 0183 target (contains all nmea0183 messages and infrastructure)
add_library(nmealib0183 STATIC
    ../nmea0183.cpp
    messageRegistry.cpp
    nmea0183Factory.cpp
    dbt.cpp
    gga.cpp
    gll.cpp
    gsa.cpp
    mwv.cpp
    mtw.cpp
    rmc.cpp
    vhw.cpp
    vtg.cpp
    zda.cpp
)

target_include_directories(nmealib0183 PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Expose NMEA 0183 sources to parent for nmealib composite target
set(NMEALIB_NMEA0183_SOURCES
    nmea0183.cpp
    nmea0183/messageRegistry.cpp
    nmea0183/nmea0183Factory.cpp
    nmea0183/dbt.cpp
    nmea0183/gga.cpp
    nmea0183/gll.cpp
    nmea0183/gsa.cpp
    nmea0183/mwv.cpp
    nmea0183/mtw.cpp
    nmea0183/rmc.cpp
    nmea0183/vhw.cpp
    nmea0183/vtg.cpp
    nmea0183/zda.cpp
    PARENT_SCOPE
)
