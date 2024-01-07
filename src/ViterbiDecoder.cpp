#include "ViterbiDecoder.hpp"
#include "logging/Logging.hpp"
#include "utils.hpp"
#include <sstream>
#include <iomanip>

ViterbiDecoder::ViterbiDecoder()
{
    const uint8_t POLY_G1 = 0x4f; // 100 1111 <- in
    const uint8_t POLY_G2 = 0x6d; // 110 1101 <- in

    for(int yyyyy=0; yyyyy<32; yyyyy++){
        // MSB and LSB of POLY_G1 and POLY_G2 are 1.
        // This means that output symbols of the following registers are same.
        //  0yyyyy0 (original was x0yyyyy, and 0 was input)
        //  1yyyyy1 (original was x1yyyyy, and 1 was input)
        // Now that output symbols are calculated from yyyyy only, and it depends on each POLY_G1 and POLY_G2.
        symbols_[0][yyyyy] = parity((yyyyy<<1) & POLY_G1) ? 255 : 0;
        symbols_[1][yyyyy] = parity((yyyyy<<1) & POLY_G2) ? 255 : 0;
        // If the register is 0yyyyy1 or 1yyyyy0, the output symbols become a complement 255-symbols_[0/1][yyyyy].
    }

    old_costs_ = &costs_[0][0];
    new_costs_ = &costs_[1][0];
    
    old_costs_[0] = 0;
    for (int i=1; i<64; i++) old_costs_[i] = 510;
}

ViterbiDecoder::~ViterbiDecoder()
{
    if (survived_path_) delete[] survived_path_;
    if (decoded_bytes_) delete[] decoded_bytes_;
}

void ViterbiDecoder::CalcCosts(uint8_t yyyyy, uint8_t symbol0, uint8_t symbol1, uint64_t* survived_path)
{
    int cost = (symbols_[0][yyyyy] ^ symbol0) + (symbols_[1][yyyyy] ^ symbol1);

    // input bit is 0
    int c0 = old_costs_[yyyyy] + cost;            // 00yyyyy to 0yyyyy0
    int c1 = old_costs_[yyyyy+32] + (510 - cost); // 10yyyyy to 0yyyyy0
    uint64_t msb = c0 > c1 ? 1 : 0;
    new_costs_[yyyyy<<1] = msb ? c1 : c0;
    *survived_path |= msb << (yyyyy<<1);

    // input bit is 1
    c0 = old_costs_[yyyyy] + (510 - cost);    // 01yyyyy to 1yyyyy1
    c1 = old_costs_[yyyyy+32] + cost;         // 11yyyyy to 1yyyyy1
    msb = c0 > c1 ? 1 : 0;
    new_costs_[(yyyyy<<1)+1] = msb ? c1 : c0;
    *survived_path |= msb << ((yyyyy<<1)+1);
}

int ViterbiDecoder::DoDecode(uint8_t *symbol_bytes, unsigned int symbol_bit_len)
{
    if (symbol_bytes == nullptr || symbol_bit_len < 2 || symbol_bit_len % 2) {
        LOG_ERROR("args are invalid");
        return -1;
    }

    decoded_bit_len_ = symbol_bit_len/2; // including 6 bits

    try {
        survived_path_ = new uint64_t[decoded_bit_len_];
        decoded_bytes_ = new uint8_t[(decoded_bit_len_-6)/8];
    } catch (...) {
        LOG_CRITICAL("Failed to new survived path");
        return -1;
    }

    for (int bit=0; bit<decoded_bit_len_; bit++) {
        uint8_t symbol0 = symbol_bytes[bit*2/8] & (1 << (7 - (bit*2 % 8))) ? 255 : 0;
        uint8_t symbol1 = symbol_bytes[(bit*2+1)/8] & (1 << (7 - ((bit*2+1)) % 8)) ? 255 : 0;
        
        survived_path_[bit] = 0;
        for (uint8_t yyyyy=0; yyyyy<32; yyyyy++) {
            CalcCosts(yyyyy, symbol0, symbol1, &survived_path_[bit]);
        }

        uint16_t* tmp_costs_ = old_costs_;
        old_costs_ = new_costs_;
        new_costs_ = tmp_costs_;
    }

    uint8_t register_state = 0;
    for (int bit=(decoded_bit_len_-1); bit>=6; bit--) {
        uint8_t msb = (survived_path_[bit] >> (register_state >> 2)) & 1;
        decoded_bytes_[(bit-6)/8] = register_state = (msb << 7) | (register_state >> 1);
    }

    std::stringstream ss;
    for (int i=0; i<(decoded_bit_len_-6)/8; i++)
        ss << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)decoded_bytes_[i];
    LOG_INFO("Decoded: %s", ss.str());

    return 0;
}

int ViterbiDecoder::GetChannelBytes(uint8_t* buf, const int bufLen)
{
    if (buf == nullptr || bufLen <= 0) {
        LOG_ERROR("args are invalid");
        return -1;
    }
    if (decoded_bytes_ == nullptr) {
        LOG_ERROR("DoDecode() should be set before calling this function");
        return -1;
    }

    int remain = (decoded_bit_len_-6)/8 - channel_bytes_seek_;
    if (remain <= 0) {
        return 0;
    }

    uint16_t len = std::min(bufLen, remain);
    try {
        std::memcpy(buf, decoded_bytes_ + channel_bytes_seek_, len);
        channel_bytes_seek_ += len;
    } catch (...) {
        LOG_CRITICAL("Failed to set m_symbols");
        return -1;
    }

    return len;
}