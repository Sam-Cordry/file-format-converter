///
/// @file tiff.c
/// @brief TIFF file format source file
/// @author Sam Cordry

// include header file
#include "tiff.h"

/// @brief The MEM_CHECK macro checks if the given pointer is NULL.
#define MEM_CHECK(ptr) if(ptr == NULL) { printf("Unable to allocate memory");\
                                            return false; }

/// @brief The FEOF_CHECK macro checks if the end of the file has been reached.
#define FEOF_CHECK(file) if(feof(file)) { printf("Unexpected end of file");\
                                            return false; }

/// @brief The skip_ahead function skips ahead in the file by the given amount.
/// @param file The file to skip ahead in.
/// @param amount The amount to skip ahead.
/// @return True if the file was successfully skipped ahead, false otherwise.
bool skip_ahead(FILE* file, int amount) {
    int i;
    for(i = 0; i < amount; i++) {
        fgetc(file);
        FEOF_CHECK(file);
    }

    return true;
}

/// @brief The is_little_tiff_header function checks if the given header is a
///        little-endian TIFF header.
/// @param header The header to check.
/// @return True if the header is a little-endian TIFF header, false otherwise.
bool is_little_tiff_header(const char* header) {
    return !(header == NULL || strlen(header) != 2 ||
                memcmp(header, TIFF_HEADER_LITTLE, 2) != 0);
}

/// @brief The is_big_tiff_header function checks if the given header is a
///        big-endian TIFF header.
/// @param header The header to check.
/// @return True if the header is a big-endian TIFF header, false otherwise.
bool is_big_tiff_header(const char* header) {
    return !(header == NULL || strlen(header) != 2 ||
                memcmp(header, TIFF_HEADER_BIG, 2) != 0);
}

/// @brief The is_ifd_header function checks if the given header is an IFD
///        header.
/// @param header The header to check.
/// @return True if the header is an IFD header, false otherwise.
bool is_ifd_header(const char* header) {
    return !(header == NULL || strlen(header) != 2 ||
                memcmp(header, IFD_HEADER, 2) != 0);
}

/// @brief The read_ifd function reads an IFD from the given file.
/// @param ifd The IFD to read into.
/// @param file The file to read from.
/// @return True if the IFD was successfully read, false otherwise.
bool read_ifd(IFD* ifd, FILE* file) {
    return false;
}

/// @brief The tiff_create function creates a TIFF structure.
/// @return The created TIFF structure.
TIFF* tiff_create() {
    // allocate memory for TIFF structure
    TIFF* tiff = malloc(sizeof(TIFF));
    MEM_CHECK(tiff);

    // set default values
    tiff->ifd = NULL;

    // return created TIFF structure
    return tiff;
}

/// @brief The tiff_read function reads a TIFF file from the given file.
/// @param tiff The TIFF structure to read into.
/// @param file The file to read from.
/// @return True if the TIFF file was successfully read, false otherwise.
bool tiff_read(TIFF* tiff, FILE* file) {
}

/// @brief The tiff_write function writes a TIFF file to the given file.
/// @param tiff The TIFF structure to write from.
/// @param file The file to write to.
/// @return True if the TIFF file was successfully written, false otherwise.
bool tiff_write(TIFF* tiff, FILE* file) {
}

/// @brief The tiff_free function frees the given TIFF structure.
/// @param tiff The TIFF structure to free.
void tiff_free(TIFF* tiff) {
    // check if TIFF structure exists
    if(tiff == NULL)
        return;

    // free IFD if it exists
    if(tiff->ifd != NULL)
        free(tiff->ifd);
    
    // free TIFF structures
    free(tiff);
}
