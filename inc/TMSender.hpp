#pragma once

#include <stdint.h>

class TMSender
{
public:
    TMSender();
    ~TMSender();
    int SetTransferFrame(const uint8_t* data, const int dataLen);
    int CreateCADU();
    int GetChannelBytes(uint8_t* buf, const int bufLen);
    const uint8_t ASM[4] = {0x1A, 0xCF, 0xFC, 0x1D};

private:
    uint8_t* m_transferFrame = nullptr;
    uint8_t* m_CADU = nullptr;
    uint8_t* m_channelBytes = nullptr;
    uint16_t m_transferFrameLen = 0;
    uint16_t m_CADULen = 0;
    uint16_t m_channelBytesSeek = 0;
};