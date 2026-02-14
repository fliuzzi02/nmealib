add_executable(nmea0183_tests
    test_nmea0183.cpp
)

target_link_libraries(nmea0183_tests
    PRIVATE
        nmealib          # your library target
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(nmea0183_tests
    PROPERTIES LABELS "nmea0183"
)
