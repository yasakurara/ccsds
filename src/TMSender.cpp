#include "TMSender.hpp"
#include "logging/Logging.hpp"

TMSender::TMSender()
{

}

TMSender::~TMSender()
{
    if (m_transferFrame) delete m_transferFrame;
    if (m_channelBytes) delete m_channelBytes;
}

int TMSender::SetTransferFrame(const uint8_t* data, const int dataLen)
{
    try {
        m_transferFrame = new uint8_t[dataLen];
        std::memcpy(m_transferFrame, data, dataLen);
    } catch (...) {
        LOG_CRITICAL("Failed to new transferFrame");
        return -1;
    }
    return 0;
}

int TMSender::GetChannelBytes(const uint8_t& buf, const int bufLen)
{
    return 0;
}