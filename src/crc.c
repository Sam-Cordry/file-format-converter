///
/// @file crc.c
/// @brief CRC implementation
/// @author PNG Specification

#include "crc.h"

// table to store already calculated CRC codes
unsigned long crc_table[256];

// tracks whether the table has been computed
bool crc_table_computed = false;

void make_crc_table(void) {
    unsigned long c;
    int n, k;

    for(n = 0; n < 256; n++) {
        c = (unsigned long) n;
        for(k = 0; k < 8; k++) {
            if(c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[n] = c;
    }
    crc_table_computed = true;
}

unsigned long update_crc(unsigned long crc, unsigned char* buf, int len) {
    unsigned long c = crc;
    int n;

    if(!crc_table_computed)
        make_crc_table();
    
    for(n = 0; n < len; n++)
        c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
    
    return c;
}

unsigned long crc(unsigned char* buf, int len) {
    return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}
