cmake_minimum_required(VERSION 3.20)

project(nmealib
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

enable_testing()

add_subdirectory(app)
add_subdirectory(src)
add_subdirectory(tests)

option(ENABLE_STATIC_ANALYSIS "Enable static analysis (clang-tidy and cppcheck)" OFF)

if(ENABLE_STATIC_ANALYSIS)
    message(STATUS "Static analysis enabled (ENABLE_STATIC_ANALYSIS=ON)")

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

    # Create a helper target that runs cppcheck (if available) and then triggers a build (so clang-tidy runs)
    if(CPPCHECK_EXE)
        add_custom_target(static-analysis
            COMMAND ${CPPCHECK_EXE} --enable=all --inconclusive --std=c++20 --suppress=missingIncludeSystem --suppress=unusedFunction
                            --error-exitcode=1
                            ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/tests ${CMAKE_SOURCE_DIR}/app
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Run cppcheck and build (clang-tidy will run during build if configured)" VERBATIM)
    else()
        add_custom_target(static-analysis
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Run build (clang-tidy will run during build if configured)" VERBATIM)
    endif()
endif()