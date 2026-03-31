# Single-message NMEA 0183 targets
# add_library(dbt STATIC dbt.cpp)
# target_include_directories(dbt PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Monolithic NMEA 0183 target (contains all nmea0183 messages and infrastructure)
add_library(nmealib0183 STATIC
    ../nmea0183.cpp
    messageRegistry.cpp
    nmea0183Factory.cpp
    apb.cpp
    dbt.cpp
    dpt.cpp
    gga.cpp
    gll.cpp
    gsa.cpp
    hdg.cpp
    hdm.cpp
    mwv.cpp
    mtw.cpp
    rmb.cpp
    rma.cpp
    rmc.cpp
    vhw.cpp
    vtg.cpp
    vwr.cpp
    xdr.cpp
    xte.cpp
    zda.cpp
)

target_include_directories(nmealib0183 PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Expose NMEA 0183 sources to parent for nmealib composite target
set(NMEALIB_NMEA0183_SOURCES
    nmea0183.cpp
    nmea0183/messageRegistry.cpp
    nmea0183/nmea0183Factory.cpp
    nmea0183/apb.cpp
    nmea0183/dbt.cpp
    nmea0183/dpt.cpp
    nmea0183/gga.cpp
    nmea0183/gll.cpp
    nmea0183/gsa.cpp
    nmea0183/hdg.cpp
    nmea0183/hdm.cpp
    nmea0183/mwv.cpp
    nmea0183/mtw.cpp
    nmea0183/rmb.cpp
    nmea0183/rma.cpp
    nmea0183/rmc.cpp
    nmea0183/vhw.cpp
    nmea0183/vtg.cpp
    nmea0183/vwr.cpp
    nmea0183/xdr.cpp
    nmea0183/xte.cpp
    nmea0183/zda.cpp
    PARENT_SCOPE
)
