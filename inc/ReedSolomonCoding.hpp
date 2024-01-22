#pragma once

#include <stdint.h>

class ReedSolomonCoding
{
public:
    ReedSolomonCoding();
    ~ReedSolomonCoding();
    uint8_t GetGFAlphaPolyBits(int exponent);
    uint8_t GetGFAlphaPolyExponent(int bits);
private:
    uint8_t gf_poly_bits = 0b10000111; // x^8 = x^7+x^2+x+1
    uint8_t gf_alpha_poly_bits_[256];
    uint8_t gf_alpha_poly_exponent_[256];
};