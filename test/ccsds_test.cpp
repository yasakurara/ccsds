#include <gtest/gtest.h>

#include "TMSender.hpp"

TEST(TMSenderTest, ASMAndTransferFrame) {
  TMSender tm;
  uint8_t data[128] = {0xFF, 0xFF, 0xFF, 0xFF};
  int rc = tm.SetTransferFrame(data, sizeof(data));
  EXPECT_EQ(rc, 0);

  rc = tm.CreateCADU();
  EXPECT_EQ(rc, 0);

  uint8_t channelBytes[8] = {};
  uint8_t expectedBytes[8] = {0x1A, 0xCF, 0xFC, 0x1D, 0xFF, 0xFF, 0xFF, 0xFF};
  rc = tm.GetChannelBytes(channelBytes, sizeof(channelBytes));
  EXPECT_EQ(rc, sizeof(expectedBytes));

  auto cmp = [](uint8_t *a, uint8_t *b, int len) -> int { return std::memcmp(a, b, len); };
  EXPECT_EQ(cmp(channelBytes, expectedBytes, sizeof(channelBytes)), 0);
}