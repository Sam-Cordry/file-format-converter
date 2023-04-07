///
/// @file crc.h
/// @brief CRC header
/// @author Sam Cordry

#ifndef CRC_H
#define CRC_H

#include <stdbool.h>

// CRC functions
void make_crc_table(void);
unsigned long update_crc(unsigned long crc, unsigned char* buf, int len);
unsigned long crc(unsigned char* buf, int len);

#endif
