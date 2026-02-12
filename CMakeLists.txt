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