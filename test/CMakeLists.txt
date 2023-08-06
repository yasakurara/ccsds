enable_testing()

add_executable(
  ccsds_test
  ccsds_test.cpp
)
target_link_libraries(
  ccsds_test
  GTest::gtest_main
  ccsds
)

include(GoogleTest)
gtest_discover_tests(ccsds_test)