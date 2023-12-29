#pragma once

#include <stdint.h>

class ConvolutionalEncoder
{
public:
    ConvolutionalEncoder();
    ~ConvolutionalEncoder();
    int SetTransferFrame(const uint8_t* data, const int dataLen);
    int DoEncode();
    int GetChannelBytes(uint8_t* buf, const int bufLen);

private:
    uint8_t* m_transferFrame = nullptr;
    uint16_t m_transferFrameLen = 0;
    uint8_t* m_symbols = nullptr;
    uint16_t m_symbolsLen = 0;
    uint8_t* m_channelBytes = nullptr;
    uint16_t m_channelBytesLen = 0;
    uint16_t m_channelBytesSeek = 0;

};