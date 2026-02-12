include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(nmealib_tests
    nmea0183/test_nmea0183.cpp
)

target_link_libraries(nmealib_tests
    PRIVATE
        nmealib          # your library target
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(nmealib_tests)
