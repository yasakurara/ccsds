#pragma once

#include <stdint.h>

class TMSender
{
public:
    TMSender();
    ~TMSender();
    int SetTransferFrame(const uint8_t* data, const int dataLen);
    int GetChannelBytes(const uint8_t& buf, const int bufLen);

private:
    uint8_t* m_transferFrame = nullptr;
    uint8_t* m_channelBytes = nullptr;
};