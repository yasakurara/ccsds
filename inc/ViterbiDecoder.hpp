#pragma once

#include <stdint.h>

class ViterbiDecoder
{
public:
    ViterbiDecoder();
    ~ViterbiDecoder();
    int DoDecode(uint8_t* symbol_bytes, unsigned int symbol_bit_len);
    int GetChannelBytes(uint8_t* buf, const int bufLen);

private:
    void CalcCosts(uint8_t yyyyy, uint8_t symbol0, uint8_t symbol1, uint64_t* survived_path);
    uint8_t symbols_[2][32];
    uint16_t costs_[2][64];
    uint16_t* old_costs_ = nullptr;
    uint16_t* new_costs_ = nullptr;
    uint64_t* survived_path_ = nullptr;
    uint8_t* decoded_bytes_ = nullptr;
    uint16_t decoded_bit_len_ = 0;
    uint16_t channel_bytes_seek_ = 0;
};