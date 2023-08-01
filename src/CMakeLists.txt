set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(.)
add_subdirectory(logging)
add_library(ccsds SHARED ccsds.cpp $<TARGET_OBJECTS:logging>)