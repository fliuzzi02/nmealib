add_executable(nmea2000_tests
	test_dataTypes.cpp
	test_nmea2000.cpp
	test_pgn128259.cpp
	test_pgn127245.cpp
	test_pgn127250.cpp
	test_pgn127251.cpp
	test_pgn127257.cpp
	test_pgn129025.cpp
	test_pgn129026.cpp
	test_pgn130306.cpp
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

