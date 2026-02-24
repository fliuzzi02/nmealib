add_executable(nmealib-cli
    main.cpp
)

target_link_libraries(nmealib-cli PRIVATE
    nmealib
)

target_compile_features(nmealib-cli PRIVATE cxx_std_20)
