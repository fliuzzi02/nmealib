include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_subdirectory(nmea0183)

# Collect existing per-module test targets and create an aggregate custom target
set(TEST_TARGETS "")
if(TARGET nmea0183_tests)
  list(APPEND TEST_TARGETS nmea0183_tests)
endif()
if(TARGET nmea2000_tests)
  list(APPEND TEST_TARGETS nmea2000_tests)
endif()

if(TEST_TARGETS)
  add_custom_target(nmealib_tests DEPENDS ${TEST_TARGETS})
else()
  add_custom_target(nmealib_tests)
endif()
