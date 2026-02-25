function(_nmealib_configure_nmea2000_target target_name)
    target_include_directories(${target_name} PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    )
    target_link_libraries(${target_name} PUBLIC
        nmealib_core
    )
endfunction()

add_library(nmealib_nmea2000_base STATIC
    ../nmea2000.cpp
)
_nmealib_configure_nmea2000_target(nmealib_nmea2000_base)

add_library(nmealib_nmea2000_registry STATIC
    messageRegistry.cpp
)
_nmealib_configure_nmea2000_target(nmealib_nmea2000_registry)

add_library(nmealib_nmea2000_factory STATIC
    nmea2000Factory.cpp
)
_nmealib_configure_nmea2000_target(nmealib_nmea2000_factory)

target_link_libraries(nmealib_nmea2000_registry PUBLIC
    nmealib_nmea2000_base
)

target_link_libraries(nmealib_nmea2000_factory PUBLIC
    nmealib_nmea2000_base
    nmealib_nmea2000_registry
)

foreach(message_target IN LISTS NMEALIB_NMEA2000_MESSAGE_TARGETS)
    _nmealib_configure_nmea2000_target(${message_target})
    target_link_libraries(${message_target} PUBLIC
        nmealib_nmea2000_base
    )
endforeach()

target_link_libraries(nmealib_nmea2000_registry PRIVATE
    ${NMEALIB_NMEA2000_MESSAGE_TARGETS}
)

add_library(nmealib_nmea2000_all INTERFACE)
target_link_libraries(nmealib_nmea2000_all INTERFACE
    nmealib_nmea2000_base
    nmealib_nmea2000_registry
    nmealib_nmea2000_factory
    ${NMEALIB_NMEA2000_MESSAGE_TARGETS}
)
target_include_directories(nmealib_nmea2000_all INTERFACE
    ${PROJECT_SOURCE_DIR}/include
)

add_library(nmealib_nmea2000 INTERFACE)
target_link_libraries(nmealib_nmea2000 INTERFACE
    nmealib_nmea2000_all
)
target_include_directories(nmealib_nmea2000 INTERFACE
    ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS
    nmealib_nmea2000_base
    nmealib_nmea2000_registry
    nmealib_nmea2000_factory
    ${NMEALIB_NMEA2000_MESSAGE_TARGETS}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    COMPONENT library
)
