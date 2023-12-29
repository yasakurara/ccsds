set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(logging)
add_library(ccsds SHARED $<TARGET_OBJECTS:logging> TMSender.cpp ConvolutionalEncoder.cpp)