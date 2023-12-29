#include "ConvolutionalEncoder.hpp"
#include "logging/Logging.hpp"
#include "utils.hpp"

ConvolutionalEncoder::ConvolutionalEncoder()
{

}

ConvolutionalEncoder::~ConvolutionalEncoder()
{
    if (m_transferFrame) delete[] m_transferFrame;
    if (m_symbols) delete[] m_symbols;
    if (m_channelBytes) delete[] m_channelBytes;
}

int ConvolutionalEncoder::SetTransferFrame(const uint8_t* data, const int dataLen)
{
    if (data == nullptr || dataLen <= 0) {
        LOG_ERROR("args are invalid");
        return -1;
    }

    try {
        m_transferFrameLen = dataLen;
        m_transferFrame = new uint8_t[m_transferFrameLen];
        std::memcpy(m_transferFrame, data, m_transferFrameLen);

        m_symbolsLen = (8*dataLen+6)*2;
        m_symbols = new uint8_t[m_symbolsLen];
        
        m_channelBytesLen = dataLen*2;
        m_channelBytes = new uint8_t[m_channelBytesLen];
    } catch (...) {
        LOG_CRITICAL("Failed to set m_transferFrame");
        return -1;
    }
    return 0;
}

int ConvolutionalEncoder::DoEncode()
{
    // G1 = 1111001 and G2 = 1011011 are defined in CCSDS 131.0-B-4 "TM SYNCHRONIZATION AND CHANNEL CODING RECOMMENDED STANDARD".
    // In "Basic Convolutional Encoder Block Diagram", the left bit of the buf is LSB. So, we need to turn over the bits of them.

    const uint8_t POLY_G1 = 0x4f; // 0100 1111
    const uint8_t POLY_G2 = 0x6d; // 0110 1101

    uint8_t buf = 0;
    for (int i=0; i<m_transferFrameLen; i++) {
        uint8_t byte = m_transferFrame[i];
        for (int bit=0; bit<8; bit++) {
            buf = (buf << 1) | ((byte >> (7-bit)) & 1);
            m_symbols[2*(i*8+bit)+0] = parity(buf & POLY_G1)*255;
            m_symbols[2*(i*8+bit)+1] = parity(buf & POLY_G2)*255;
        }
    }
    for (int bit=0; bit<6; bit++) {
        buf = buf << 1;
        m_symbols[2*(m_transferFrameLen*8+bit)+0] = parity(buf & POLY_G1)*255;
        m_symbols[2*(m_transferFrameLen*8+bit)+1] = parity(buf & POLY_G2)*255;
    }

    for (int i=0; i<m_channelBytesLen; i++) {
        uint8_t byte = 0;
        for (int j=0; j<8; j++) {
            if (m_symbols[i*8+j]) byte |= 1 << (7-j);
        }
        m_channelBytes[i] = byte;
    }

    return 0;
}

int ConvolutionalEncoder::GetChannelBytes(uint8_t* buf, const int bufLen)
{
    if (buf == nullptr || bufLen <= 0) {
        LOG_ERROR("args are invalid");
        return -1;
    }
    if (m_symbols == nullptr) {
        LOG_ERROR("SetTransferFrame() should be set before calling this function");
        return -1;
    }

    int remain = m_channelBytesLen - m_channelBytesSeek;
    if (remain <= 0) {
        return 0;
    }

    uint16_t len = std::min(bufLen, remain);
    try {
        std::memcpy(buf, m_channelBytes + m_channelBytesSeek, len);
        m_channelBytesSeek += len;
    } catch (...) {
        LOG_CRITICAL("Failed to set m_symbols");
        return -1;
    }

    return len;
}