cmake_minimum_required(VERSION 3.20)

# in CMakeLists.txt
file(READ "${CMAKE_SOURCE_DIR}/VERSION" NMEALIB_VERSION)
string(STRIP "${NMEALIB_VERSION}" NMEALIB_VERSION)

project(nmealib VERSION "${NMEALIB_VERSION}" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set parallel build level to 4 by default (can be overridden by environment variable or command line)
if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
    set(CMAKE_BUILD_PARALLEL_LEVEL 16)
endif()

# Code coverage options
option(ENABLE_COVERAGE "Enable code coverage instrumentation" OFF)

if(ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled (ENABLE_COVERAGE=ON)")
    
    # Coverage flags for GCC/Clang - applied to all build types
    add_compile_options(-fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
    
    message(STATUS "Coverage flags added via add_compile_options and add_link_options")
endif()

add_subdirectory(src)

option(BUILD_CLI "Build the CLI app" ON)
if(BUILD_CLI)
  add_subdirectory(app)
endif()

# Install public headers
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include
    COMPONENT library
)

# Install CMake config file for find_package(nmealib) support
install(FILES ${PROJECT_SOURCE_DIR}/scripts/nmealibConfig.cmake
    DESTINATION lib/cmake/nmealib
    COMPONENT library
)

# Install license and readme
install(FILES ${PROJECT_SOURCE_DIR}/LICENSE ${PROJECT_SOURCE_DIR}/README.md
    DESTINATION .
    COMPONENT library
)

if(BUILD_CLI)
    # Install cli application
    install(TARGETS nmealib-cli
        RUNTIME DESTINATION bin
        COMPONENT cli
    )
endif()

# Unit testing options
option(ENABLE_TESTS "Enable building and running tests" OFF)

if(ENABLE_TESTS)
    message(STATUS "Tests enabled (ENABLE_TESTS=ON)")
    include(CTest)

    enable_testing()

    add_subdirectory(tests EXCLUDE_FROM_INSTALL)
endif()

# Static analysis options
option(ENABLE_STATIC_ANALYSIS "Enable static analysis (clang-tidy and cppcheck)" OFF)

if(ENABLE_STATIC_ANALYSIS)
    message(STATUS "Static analysis enabled (ENABLE_STATIC_ANALYSIS=ON)")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile commands for static analysis tools" FORCE)

    find_program(CLANG_TIDY_EXE NAMES clang-tidy clang-tidy-14 clang-tidy-13)
    if(CLANG_TIDY_EXE)
        # Run clang-tidy as part of the compilation step
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=*,clang-analyzer-*,-readability-function-cognitive-complexity" CACHE STRING "clang-tidy executable and options" FORCE)
        message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
    else()
        message(WARNING "clang-tidy not found; clang-tidy checks will not run during build")
    endif()

    find_program(CPPCHECK_EXE NAMES cppcheck)
    if(CPPCHECK_EXE)
        message(STATUS "Found cppcheck: ${CPPCHECK_EXE}")
    else()
        message(WARNING "cppcheck not found; cppcheck will be skipped")
    endif()

    if(CPPCHECK_EXE)
        set(CPPCHECK_REPORTS_DIR "${CMAKE_BINARY_DIR}/reports")
        file(MAKE_DIRECTORY "${CPPCHECK_REPORTS_DIR}")
        file(WRITE "${CMAKE_BINARY_DIR}/run_cppcheck.cmake"
            "string(TIMESTAMP NOW \"%Y%m%d-%H%M%S\")\n"
                "set(REPORT_FILE \"${CPPCHECK_REPORTS_DIR}/cppcheck-\${NOW}.xml\")\n"
                "set(COMPILE_COMMANDS_FILE \"${CMAKE_BINARY_DIR}/compile_commands.json\")\n"
                "message(STATUS \"Running cppcheck; report: \${REPORT_FILE}\")\n"
            "if(NOT EXISTS \"\${COMPILE_COMMANDS_FILE}\")\n"
                "    message(FATAL_ERROR \"compile_commands.json not found at \${COMPILE_COMMANDS_FILE}\")\n"
            "endif()\n"
            "execute_process(\n"
                "    COMMAND \"${CPPCHECK_EXE}\" --project=\${COMPILE_COMMANDS_FILE} --enable=all --inconclusive --std=c++20 --suppress=missingIncludeSystem --suppress=unusedFunction --error-exitcode=1 --xml --xml-version=2 --output-file=\${REPORT_FILE}\n"
            "    RESULT_VARIABLE CPPCHECK_RESULT\n"
            ")\n"
            "if(NOT CPPCHECK_RESULT EQUAL 0)\n"
                "    message(FATAL_ERROR \"cppcheck failed with exit code \${CPPCHECK_RESULT}\")\n"
            "endif()\n"
        )

        add_custom_target(static-analysis
            COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/run_cppcheck.cmake"
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Run cppcheck (timestamped report) and build (clang-tidy will run during build if configured)" VERBATIM)
    else()
        add_custom_target(static-analysis
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Run build (clang-tidy will run during build if configured)" VERBATIM)
    endif()
endif()
