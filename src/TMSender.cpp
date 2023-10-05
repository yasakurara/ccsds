#include "TMSender.hpp"
#include "logging/Logging.hpp"

TMSender::TMSender()
{

}

TMSender::~TMSender()
{
    if (m_transferFrame) delete m_transferFrame;
    if (m_CADU) delete m_CADU;
    if (m_channelBytes) delete m_channelBytes;
}

int TMSender::SetTransferFrame(const uint8_t* data, const int dataLen)
{
    if (data == nullptr || dataLen <= 0) {
        LOG_ERROR("args are invalid");
        return -1;
    }

    try {
        m_transferFrame = new uint8_t[dataLen];
        std::memcpy(m_transferFrame, data, dataLen);
        m_transferFrameLen = dataLen;
    } catch (...) {
        LOG_CRITICAL("Failed to set m_transferFrame");
        return -1;
    }
    return 0;
}

int TMSender::CreateCADU()
{
    if (m_transferFrame == nullptr) {
        LOG_ERROR("Transfer Frame should be set before calling this function");
        return -1;
    }

    m_CADULen = sizeof(ASM) + m_transferFrameLen;
    try {
        m_CADU = new uint8_t[m_CADULen];
        std::memcpy(m_CADU, ASM, sizeof(ASM));
        std::memcpy(m_CADU+sizeof(ASM), m_transferFrame, m_transferFrameLen);
    } catch (...) {
        LOG_CRITICAL("Failed to set m_CADU");
        return -1;
    }
    return 0;
}

int TMSender::GetChannelBytes(uint8_t* buf, const int bufLen)
{
    if (buf == nullptr || bufLen<= 0) {
        LOG_ERROR("args are invalid");
        return -1;
    }
    if (m_CADU == nullptr) {
        LOG_ERROR("CADU should be set before calling this function");
        return -1;
    }

    int remain = m_CADULen - m_channelBytesSeek;
    if (remain <= 0) {
        return 0;
    }

    uint16_t len = std::min(bufLen, remain);
    try {
        std::memcpy(buf, m_CADU + m_channelBytesSeek, len);
        m_channelBytesSeek += len;
    } catch (...) {
        LOG_CRITICAL("Failed to set m_CADU");
        return -1;
    }

    return len;
}