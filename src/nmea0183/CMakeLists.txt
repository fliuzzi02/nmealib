function(_nmealib_configure_nmea0183_target target_name)
    target_include_directories(${target_name} PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    )
    target_link_libraries(${target_name} PUBLIC
        nmealib_core
    )
    target_compile_features(${target_name} PUBLIC cxx_std_20)
endfunction()

# Common NMEA 0183 base parsing/type support
add_library(nmealib_nmea0183_base STATIC
    ../nmea0183.cpp
)
_nmealib_configure_nmea0183_target(nmealib_nmea0183_base)

# Registry and factory
add_library(nmealib_nmea0183_registry STATIC
    messageRegistry.cpp
)
_nmealib_configure_nmea0183_target(nmealib_nmea0183_registry)

add_library(nmealib_nmea0183_factory STATIC
    nmea0183Factory.cpp
)
_nmealib_configure_nmea0183_target(nmealib_nmea0183_factory)

target_link_libraries(nmealib_nmea0183_registry PUBLIC
    nmealib_nmea0183_base
)

target_link_libraries(nmealib_nmea0183_factory PUBLIC
    nmealib_nmea0183_base
    nmealib_nmea0183_registry
)

# Per-message static libraries
add_library(nmealib_nmea0183_dbt STATIC dbt.cpp)
add_library(nmealib_nmea0183_gga STATIC gga.cpp)
add_library(nmealib_nmea0183_gsa STATIC gsa.cpp)
add_library(nmealib_nmea0183_gll STATIC gll.cpp)
add_library(nmealib_nmea0183_mwv STATIC mwv.cpp)
add_library(nmealib_nmea0183_rmc STATIC rmc.cpp)
add_library(nmealib_nmea0183_vtg STATIC vtg.cpp)
add_library(nmealib_nmea0183_zda STATIC zda.cpp)

set(NMEALIB_NMEA0183_MESSAGE_TARGETS
    nmealib_nmea0183_dbt
    nmealib_nmea0183_gga
    nmealib_nmea0183_gsa
    nmealib_nmea0183_gll
    nmealib_nmea0183_mwv
    nmealib_nmea0183_rmc
    nmealib_nmea0183_vtg
    nmealib_nmea0183_zda
)

foreach(message_target IN LISTS NMEALIB_NMEA0183_MESSAGE_TARGETS)
    _nmealib_configure_nmea0183_target(${message_target})
    target_link_libraries(${message_target} PUBLIC
        nmealib_nmea0183_base
    )
endforeach()

# Registry needs all currently implemented sentence types
target_link_libraries(nmealib_nmea0183_registry PUBLIC
    ${NMEALIB_NMEA0183_MESSAGE_TARGETS}
)

# Aggregate target (all NMEA 0183 features)
add_library(nmealib_nmea0183_all INTERFACE)
target_link_libraries(nmealib_nmea0183_all INTERFACE
    nmealib_nmea0183_base
    nmealib_nmea0183_registry
    nmealib_nmea0183_factory
    ${NMEALIB_NMEA0183_MESSAGE_TARGETS}
)
target_include_directories(nmealib_nmea0183_all INTERFACE
    ${PROJECT_SOURCE_DIR}/include
)

# Backward-compatible alias for existing consumers
add_library(nmealib_nmea0183 INTERFACE)
target_link_libraries(nmealib_nmea0183 INTERFACE
    nmealib_nmea0183_all
)
target_include_directories(nmealib_nmea0183 INTERFACE
    ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS
    nmealib_nmea0183_base
    nmealib_nmea0183_registry
    nmealib_nmea0183_factory
    ${NMEALIB_NMEA0183_MESSAGE_TARGETS}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    COMPONENT library
)
