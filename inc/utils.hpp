#pragma once

#include <stdint.h>

// odd-parity
// return 1 if total number of 1 in the byte is odd number.
static uint8_t parity(uint8_t byte) {
    byte = byte ^ (byte >> 4); // 76543210 xor xxxx7654 => xxxx(7^3)(6^2)(5^1)(4^0)
    byte = byte ^ (byte >> 2); // xxxx(7^3)(6^2)(5^1)(4^0) xor xxxxxx(7^3)(6^2) => xxxxxx(7^3^5^1)(6^2^4^0)
    byte = byte ^ (byte >> 1); // xxxxxx(7^3^5^1)(6^2^4^0) xor xxxxxxx(7^3^5^1) => xxxxxxx(7^6^5^4^3^2^1^0)
    return (byte & 1);
}

static unsigned char parity_tab[256];

static void init_parity_tab(void){
  for(int i=0;i<256;i++){
    int cnt = 0;
    int ti = i;
    while(ti){
      if(ti & 1) cnt++;
      ti >>= 1;
    }
    parity_tab[i] = cnt & 1;
  }
}