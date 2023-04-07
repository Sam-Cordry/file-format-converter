///
/// @file png.h
/// @brief PNG file format reader and writer
/// @author Sam Cordry

#ifndef PNG_H
#define PNG_H

// include all necessary system libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// define png headers
#define PNG_HEADER "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"
#define IHDR_HEADER "\x49\x48\x44\x52"
#define PLTE_HEADER "\x50\x4C\x54\x45"
#define IDAT_HEADER "\x49\x44\x41\x54"
#define IEND_HEADER "\x49\x45\x4E\x44"
#define IEND_CRC "\xAE\x42\x60\x82"

/// @brief IHDR chunk
typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned char bit_depth;
    unsigned char color_type;
    unsigned char compression_method;
    unsigned char filter_method;
    unsigned char interlace_method;
    unsigned char crc[5];
} IHDR;

/// @brief PLTE chunk
typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char crc[5];
} PLTE;

/// @brief IDAT chunk
typedef struct {
    unsigned char* data;
    unsigned int length;
    unsigned char crc[5];
} IDAT;

/// @brief IEND chunk
typedef struct {
    unsigned char crc[5];
} IEND;

/// @brief PNG file with all preceding chunks
typedef struct {
    IHDR* ihdr;
    PLTE* plte;
    IDAT* idat;
    IEND* iend;
    unsigned int num_idat_chunks;
} PNG;

// header checker functions
bool is_png_header(const char* header);
bool is_ihdr_header(const char* header);
bool is_plte_header(const char* header);
bool is_idat_header(const char* header);
bool is_iend_header(const char* header);

// chunk reader functions
bool read_ihdr(PNG* png, FILE* file, int length);
bool read_plte(PNG* png, FILE* file, int length);
bool read_idat(PNG* png, FILE* file, int length);
bool read_iend(PNG* png, FILE* file);

// chunk writer functions
bool write_ihdr(PNG* png, FILE* file);
bool write_plte(PNG* png, FILE* file);
bool write_idat(PNG* png, FILE* file);
bool write_iend(PNG* png, FILE* file);

// PNG functions
PNG* png_create(void);
bool png_read(PNG* png, FILE* file);
bool png_write(PNG* png, FILE* file);
void png_free(PNG* png);

#endif
