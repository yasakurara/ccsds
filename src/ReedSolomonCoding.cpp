#include "ReedSolomonCoding.hpp"
#include "logging/Logging.hpp"
#include "utils.hpp"

ReedSolomonCoding::ReedSolomonCoding()
{
    uint8_t mask = 1;
    gf_alpha_poly_bits_[0] = mask;
    gf_alpha_poly_exponent_[mask] = 0;

    for (int exponent=1; exponent<255; exponent++) {
        if (mask & 0b10000000) {
            gf_alpha_poly_bits_[exponent] = mask = (mask << 1) ^ gf_poly_bits;
        } else {
            gf_alpha_poly_bits_[exponent] = mask = (mask << 1);
        }
        gf_alpha_poly_exponent_[mask] = exponent;
    }

    gf_alpha_poly_bits_[255] = 0;
    gf_alpha_poly_exponent_[0] = 255;


    /* (a^(11*112) + X) */
    rs_poly_exponent_[0] = gf_alpha_poly_bits_[mod255(11*112)];
    rs_poly_exponent_[1] = 1;

    for (int i = 2; i <= 32; i++) {
        rs_poly_exponent_[i] = 1;

        /* (a^(11*112) + X)(a^(11*113) + X)... */
        for (int j = i - 1; j > 0; j--)
            if (rs_poly_exponent_[j] != 0)
                rs_poly_exponent_[j] = rs_poly_exponent_[j - 1] ^ gf_alpha_poly_bits_[mod255((gf_alpha_poly_exponent_[rs_poly_exponent_[j]]) + (112 + i - 1)*11)];
            else
                rs_poly_exponent_[j] = rs_poly_exponent_[j - 1];
        rs_poly_exponent_[0] = gf_alpha_poly_bits_[mod255((gf_alpha_poly_exponent_[rs_poly_exponent_[0]]) + (112 + i - 1)*11)];
    }
    for (int i=0; i<33; i++) rs_poly_exponent_[i] = gf_alpha_poly_exponent_[rs_poly_exponent_[i]];
}

ReedSolomonCoding::~ReedSolomonCoding()
{

}

uint8_t ReedSolomonCoding::GetGFAlphaPolyBits(int exponent)
{
    return gf_alpha_poly_bits_[exponent];
}

uint8_t ReedSolomonCoding::GetGFAlphaPolyExponent(int bits)
{
    return gf_alpha_poly_exponent_[bits];
}

uint8_t ReedSolomonCoding::GetRSPolyExponent(int index)
{
    return rs_poly_exponent_[index];
}