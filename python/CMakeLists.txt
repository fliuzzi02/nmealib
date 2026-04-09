find_package(pybind11 CONFIG REQUIRED)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

option(NMEALIB_GENERATE_PYTHON_STUBS "Generate .pyi stubs for Python IntelliSense" OFF)

pybind11_add_module(_core MODULE src/bindings.cpp)

target_link_libraries(_core PRIVATE nmealib)

target_include_directories(_core PRIVATE ${PROJECT_SOURCE_DIR}/include)

set_target_properties(_core PROPERTIES OUTPUT_NAME "_core")

install(TARGETS _core
    LIBRARY DESTINATION nmealib
    RUNTIME DESTINATION nmealib
    ARCHIVE DESTINATION nmealib
)

add_custom_target(python-stubs
    COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/scripts/generate_python_stubs.py
            --module nmealib._core
            --package-dir ${PROJECT_SOURCE_DIR}/python/nmealib
            --python ${Python3_EXECUTABLE}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    DEPENDS _core
    COMMENT "Generate Python .pyi stubs for nmealib"
    VERBATIM
)

if(NMEALIB_GENERATE_PYTHON_STUBS)
    add_custom_command(TARGET _core POST_BUILD
        COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/scripts/generate_python_stubs.py
                --module nmealib._core
                --package-dir ${PROJECT_SOURCE_DIR}/python/nmealib
                --python ${Python3_EXECUTABLE}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Auto-generating Python .pyi stubs for nmealib"
        VERBATIM
    )
endif()
