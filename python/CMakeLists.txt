find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_core MODULE src/bindings.cpp)

target_link_libraries(_core PRIVATE nmealib)

target_include_directories(_core PRIVATE ${PROJECT_SOURCE_DIR}/include)

set_target_properties(_core PROPERTIES OUTPUT_NAME "_core")

install(TARGETS _core
    LIBRARY DESTINATION nmealib
    RUNTIME DESTINATION nmealib
    ARCHIVE DESTINATION nmealib
)
