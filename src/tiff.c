#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tiff.h"

#define MEM_CHECK(ptr) if(ptr == NULL) { printf("Unable to allocate memory");\
                                            return false; }
#define FEOF_CHECK(file) if(feof(file)) { printf("Unexpected end of file");\
                                            return false; }

bool skip_ahead(FILE* file, int amount) {
    int i;
    for(i = 0; i < amount; i++) {
        fgetc(file);
        FEOF_CHECK(file);
    }

    return true;
}

bool is_little_tiff_header(const char* header) {
    return !(header == NULL || strlen(header) != 2 ||
                memcmp(header, TIFF_HEADER_LITTLE, 2) != 0);
}

bool is_big_tiff_header(const char* header) {
    return !(header == NULL || strlen(header) != 2 ||
                memcmp(header, TIFF_HEADER_BIG, 2) != 0);
}

bool is_ifd_header(const char* header) {
    return !(header == NULL || strlen(header) != 2 ||
                memcmp(header, IFD_HEADER, 2) != 0);
}

bool read_ifd(IFD* ifd, FILE* file) {
    return false;
}

TIFF* tiff_create() {
    TIFF* tiff = malloc(sizeof(TIFF));
    if(tiff == NULL) {
        printf("Unable to allocate memory");
        return NULL;
    }

    tiff->ifd = NULL;

    return tiff;
}

bool tiff_read(TIFF* tiff, FILE* file) {
}

bool tiff_write(TIFF* tiff, FILE* file) {
}

void tiff_free(TIFF* tiff) {
    if(tiff == NULL)
        return;

    if(tiff->ifd != NULL)
        free(tiff->ifd);
    
    free(tiff);
}
