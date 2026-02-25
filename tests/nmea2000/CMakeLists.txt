add_executable(nmea2000_tests
	test_nmea2000.cpp
	test_PGN129029.cpp
)

target_link_libraries(nmea2000_tests
	PRIVATE
		nmealib
		GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(nmea2000_tests
	PROPERTIES LABELS "nmea2000"
)

