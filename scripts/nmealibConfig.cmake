# nmealibConfig.cmake
# Provides find_package(nmealib) support for CMake projects
#
# Usage:
#   find_package(nmealib REQUIRED)
#   target_link_libraries(your_app PRIVATE nmealib::nmealib)
# 
# Or with explicit PATH:
#   find_package(nmealib REQUIRED PATHS /path/to/nmealib-1.0.0/lib/cmake/nmealib)

# Determine package directory
# Config file is at: <prefix>/lib/cmake/nmealib/nmealibConfig.cmake
# So: CMAKE_CURRENT_LIST_DIR/../.. = <prefix>/lib
# Therefore: CMAKE_CURRENT_LIST_DIR/../../.. = <prefix>
get_filename_component(NMEALIB_PACKAGE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)

# Locate library
find_library(NMEALIB_LIBRARY
    NAMES nmealib
    PATHS "${NMEALIB_PACKAGE_DIR}/lib"
    NO_DEFAULT_PATH
    REQUIRED
)

# Include directory
set(NMEALIB_INCLUDE_DIR "${NMEALIB_PACKAGE_DIR}/include")

if(NOT EXISTS "${NMEALIB_INCLUDE_DIR}")
    message(FATAL_ERROR "nmealib include directory not found: ${NMEALIB_INCLUDE_DIR}")
endif()

# Create imported target
if(NOT TARGET nmealib::nmealib)
    add_library(nmealib::nmealib STATIC IMPORTED)
    set_target_properties(nmealib::nmealib PROPERTIES
        IMPORTED_LOCATION "${NMEALIB_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${NMEALIB_INCLUDE_DIR}"
    )
endif()

# For backwards compatibility, also provide nmealib target
if(NOT TARGET nmealib)
    add_library(nmealib INTERFACE IMPORTED)
    target_link_libraries(nmealib INTERFACE nmealib::nmealib)
endif()

message(STATUS "Found nmealib: ${NMEALIB_LIBRARY}")
