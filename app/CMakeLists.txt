add_executable(nmealib-cli
    main.cpp
)

target_link_libraries(nmealib-cli PRIVATE
    nmealib
)
