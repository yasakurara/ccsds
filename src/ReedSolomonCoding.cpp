#include "ReedSolomonCoding.hpp"
#include "logging/Logging.hpp"

ReedSolomonCoding::ReedSolomonCoding()
{
    uint8_t mask = 1;
    gf_alpha_poly_bits_[0] = mask;
    for (int exponent=1; exponent<255; exponent++) {
        if (mask & 0b10000000) {
            gf_alpha_poly_bits_[exponent] = mask = (mask << 1) ^ gf_poly_bits;
        } else {
            gf_alpha_poly_bits_[exponent] = mask = (mask << 1);
        }
    }
    gf_alpha_poly_bits_[255] = 0;

}

ReedSolomonCoding::~ReedSolomonCoding()
{

}

uint8_t ReedSolomonCoding::GetGFAlphaPolyBits(int exponent)
{
    return gf_alpha_poly_bits_[exponent];
}