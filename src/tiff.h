///
/// @file tiff.h
/// @brief TIFF file format header file
/// @author Sam Cordry

#ifndef TIFF_H
#define TIFF_H

// include necessary system libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// define macros for tags
#define TIFF_HEADER_LITTLE "II"
#define TIFF_HEADER_BIG "MM"
#define IFD_HEADER "II"

/// @brief IFD structure
typedef struct {
    char tag[3]; ///< tag
    char type[3]; ///< type
    char count[5]; ///< count
    char value[5]; ///< value
} IFD;

/// @brief TIFF structure
typedef struct {
    char byte_order[3]; ///< byte order
    char offset[5]; ///< offset
    IFD* ifd; ///< IFD
} TIFF;

// check header functions
bool is_little_tiff_header(const char* header);
bool is_big_tiff_header(const char* header);
bool is_ifd_header(const char* header);

// read function
bool read_ifd(IFD* ifd, FILE* file);

// TIFF functions
TIFF* tiff_create();
bool tiff_read(TIFF* tiff, FILE* file);
bool tiff_write(TIFF* tiff, FILE* file);
void tiff_free(TIFF* tiff);

#endif
