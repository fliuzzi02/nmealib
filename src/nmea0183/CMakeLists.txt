# Single-message NMEA 0183 targets
add_library(dbt STATIC dbt.cpp)
target_include_directories(dbt PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(gga STATIC gga.cpp)
target_include_directories(gga PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(gll STATIC gll.cpp)
target_include_directories(gll PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(gsa STATIC gsa.cpp)
target_include_directories(gsa PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(hdm STATIC hdm.cpp)
target_include_directories(hdm PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(mtw STATIC mtw.cpp)
target_include_directories(mtw PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(mwv STATIC mwv.cpp)
target_include_directories(mwv PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(rmc STATIC rmc.cpp)
target_include_directories(rmc PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(vhw STATIC vhw.cpp)
target_include_directories(vhw PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(vtg STATIC vtg.cpp)
target_include_directories(vtg PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(vwr STATIC vwr.cpp)
target_include_directories(vwr PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(zda STATIC zda.cpp)
target_include_directories(zda PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Monolithic NMEA 0183 target (contains all nmea0183 messages and infrastructure)
add_library(nmealib0183 STATIC
    ../nmea0183.cpp
    messageRegistry.cpp
    nmea0183Factory.cpp
    dbt.cpp
    gga.cpp
    gll.cpp
    gsa.cpp
    hdm.cpp
    mwv.cpp
    mtw.cpp
    rmc.cpp
    vhw.cpp
    vtg.cpp
    vwr.cpp
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
    nmea0183/hdm.cpp
    nmea0183/mwv.cpp
    nmea0183/mtw.cpp
    nmea0183/rmc.cpp
    nmea0183/vhw.cpp
    nmea0183/vtg.cpp
    nmea0183/vwr.cpp
    nmea0183/zda.cpp
    PARENT_SCOPE
)
