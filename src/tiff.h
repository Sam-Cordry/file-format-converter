#ifndef TIFF_H
#define TIFF_H

#include <stdbool.h>

#define TIFF_HEADER_LITTLE "II"
#define TIFF_HEADER_BIG "MM"
#define IFD_HEADER "II"

typedef struct {
    char tag[3];
    char type[3];
    char count[5];
    char value[5];
} IFD;

typedef struct {
    char byte_order[3];
    char offset[5];
    IFD* ifd;
} TIFF;

bool is_little_tiff_header(const char* header);
bool is_big_tiff_header(const char* header);
bool is_ifd_header(const char* header);

bool read_ifd(IFD* ifd, FILE* file);

TIFF* tiff_create();
bool tiff_read(TIFF* tiff, FILE* file);
bool tiff_write(TIFF* tiff, FILE* file);
void tiff_free(TIFF* tiff);

#endif
