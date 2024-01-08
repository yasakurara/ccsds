#include <gtest/gtest.h>

#include "TMSender.hpp"

TEST(TMSenderTest, ASMAndTransferFrame) {
  TMSender tm;
  uint8_t data[128] = {0x01, 0x02, 0x03, 0x04};
  int rc = tm.SetTransferFrame(data, sizeof(data));
  EXPECT_EQ(rc, 0);

  rc = tm.CreateCADU();
  EXPECT_EQ(rc, 0);

  uint8_t channelBytes[6] = {};
  uint8_t expectedBytes[6] = {0x1A, 0xCF, 0xFC, 0x1D, 0x01, 0x02};
  rc = tm.GetChannelBytes(channelBytes, sizeof(channelBytes));
  EXPECT_EQ(rc, sizeof(expectedBytes));

  auto cmp = [](uint8_t *a, uint8_t *b, int len) -> int { return std::memcmp(a, b, len); };
  EXPECT_EQ(cmp(channelBytes, expectedBytes, sizeof(channelBytes)), 0);

  rc = tm.GetChannelBytes(channelBytes, 2);
  EXPECT_EQ(rc, 2);

  expectedBytes[0] = 0x03;
  expectedBytes[1] = 0x04;
  EXPECT_EQ(cmp(channelBytes, expectedBytes, 2), 0);
}

#include "utils.hpp"

TEST(UtilsTest, Parity) {
  EXPECT_EQ(parity(0x00), 0);
  EXPECT_EQ(parity(0x01), 1);
  EXPECT_EQ(parity(0xff), 0);

  init_parity_tab();
  for (int i=0; i<255; i++) {
    EXPECT_EQ(parity(i), parity_tab[i]);
  }
}

#include "ConvolutionalEncoder.hpp"

TEST(ConvolutionalEncoderTest, Encode) {
  ConvolutionalEncoder ce;
  uint8_t data[4] = {0x1a, 0xcf, 0xfc, 0x1d};
  int rc = ce.SetTransferFrame(data, sizeof(data));
  EXPECT_EQ(rc, 0);

  rc = ce.DoEncode();
  EXPECT_EQ(rc, 0);

  uint8_t channelBytes[10] = {};
  uint8_t expectedBytes[10] = {0x03, 0x5d, 0x49, 0xc2, 0x4f, 0xf2, 0x68, 0x6b, 0x17, 0x70};
  rc = ce.GetChannelBytes(channelBytes, sizeof(channelBytes));
  EXPECT_EQ(rc, sizeof(expectedBytes));

  auto cmp = [](uint8_t *a, uint8_t *b, int len) -> int { return std::memcmp(a, b, len); };
  EXPECT_EQ(cmp(channelBytes, expectedBytes, sizeof(channelBytes)), 0);
}

#include "ViterbiDecoder.hpp"

TEST(ViterbiDecoderTest, Decode) {
  ViterbiDecoder vd;
  uint8_t data[10] = {0x03, 0x5d, 0x49, 0xc2, 0x4f, 0xf2, 0x68, 0x6b, 0x17, 0x70};
  int rc = vd.DoDecode(data, (4*8+6)*2);
  EXPECT_EQ(rc, 0);

  uint8_t channelBytes[4] = {};
  uint8_t expectedBytes[4] = {0x1a, 0xcf, 0xfc, 0x1d};
  rc = vd.GetChannelBytes(channelBytes, sizeof(channelBytes));
  EXPECT_EQ(rc, sizeof(expectedBytes));

  auto cmp = [](uint8_t *a, uint8_t *b, int len) -> int { return std::memcmp(a, b, len); };
  EXPECT_EQ(cmp(channelBytes, expectedBytes, sizeof(channelBytes)), 0);
}

TEST(ViterbiDecoderTest, CorrectErrors) {
  ViterbiDecoder vd;
  uint8_t data[10] = {0x03, 0x5d, 0x49, 0xc2, 0x4f, 0xf2, 0x68, 0x6b, 0x17, 0x70};
  for(int i=0; i<sizeof(data); i++) data[i] ^= 0x80;
  int rc = vd.DoDecode(data, (4*8+6)*2);
  EXPECT_EQ(rc, 0);

  uint8_t channelBytes[4] = {};
  uint8_t expectedBytes[4] = {0x1a, 0xcf, 0xfc, 0x1d};
  rc = vd.GetChannelBytes(channelBytes, sizeof(channelBytes));
  EXPECT_EQ(rc, sizeof(expectedBytes));

  auto cmp = [](uint8_t *a, uint8_t *b, int len) -> int { return std::memcmp(a, b, len); };
  EXPECT_EQ(cmp(channelBytes, expectedBytes, sizeof(channelBytes)), 0);
}