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

#ifndef PNG_IMPL
typedef struct { } * PNG;
#endif

bool is_png_header(const char * header);
bool is_ihdr_header(const char * header);
bool is_plte_header(const char * header);
bool is_idat_header(const char * header);
bool is_iend_header(const char * header);

bool read_ihdr(PNG * png, FILE * file, int length);
bool read_plte(PNG * png, FILE * file, int length);
bool read_idat(PNG * png, FILE * file, int length);
bool read_iend(PNG * png, FILE * file, int length);

PNG png_init();
bool png_read_direct(PNG * png, FILE * file);
bool png_read_bitmap(PNG * png, FILE * file);
bool png_write_direct(PNG * png, FILE * file);
bool png_write_bitmap(PNG * png, FILE * file);
void png_free(PNG * png);

#endif
