set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(ccsds-cli ccsds-cli.cpp)
target_include_directories(ccsds-cli PUBLIC ${PROJECT_SOURCE_DIR}/inc)
target_link_libraries(ccsds-cli ccsds)
