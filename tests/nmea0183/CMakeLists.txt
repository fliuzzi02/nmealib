add_executable(nmea0183_tests
    test_nmea0183.cpp
    test_RMC.cpp
    test_GGA.cpp
    test_GSA.cpp
    test_GLL.cpp
    test_MWV.cpp
    test_VTG.cpp
    test_ZDA.cpp
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
