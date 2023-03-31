#ifndef PNG_H
#define PNG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PNG_HEADER "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"
#define IHDR_HEADER "\x49\x48\x44\x52"
#define PLTE_HEADER "\x50\x4C\x54\x45"
#define IDAT_HEADER "\x49\x44\x41\x54"
#define IEND_HEADER "\x49\x45\x4E\x44"

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int bit_depth;
    char color_type;
    char compression_method;
    char filter_method;
    char interlace_method;
    unsigned long crc;
} IHDR;

typedef struct {
    char red;
    char green;
    char blue;
    unsigned long crc;
} PLTE;

typedef struct {
    char* data;
    unsigned int length;
    unsigned long crc;
} IDAT;

typedef struct {
    unsigned long crc;
} IEND;

typedef struct {
    char* filename;
    IHDR* ihdr;
    PLTE* plte;
    IDAT* idat;
    IEND* iend;
    unsigned int num_idat_chunks;
} PNG;

bool is_png_header(const char* header);
bool is_ihdr_header(const char* header);
bool is_plte_header(const char* header);
bool is_idat_header(const char* header);
bool is_iend_header(const char* header);

bool read_ihdr(PNG* png, FILE* file, int length);
bool read_plte(PNG* png, FILE* file, int length);
bool read_idat(PNG* png, FILE* file, int length);
bool read_iend(PNG* png, FILE* file);

PNG* png_create();
bool png_read_direct(PNG* png, FILE* file);
bool png_read_bitmap(PNG* png, FILE* file);
bool png_write_direct(PNG* png, FILE* file);
bool png_write_bitmap(PNG* png, FILE* file);
void png_free(PNG* png);

#endif
