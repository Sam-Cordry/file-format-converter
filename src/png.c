#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MEM_CHECK(ptr) if(ptr == NULL) { perror("Unable to allocate memory.");\
                                            return false; }
#define FEOF_CHECK(file) if(feof(file)) { perror("Unexpected end of file.");\
                                            return false; }

typedef struct {
    char * filename;
    IHDR * ihdr;
    PLTE * plte;
    IDAT * idat;
    IEND * iend;
    unsigned int num_idat_chunks;
} structPNG;

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int bit_depth;
    char color_type;
    char compression_method;
    char filter_method;
    char interlace_method;
    unsigned long crc;
} structIHDR;

typedef struct {
    char red;
    char green;
    char blue;
    unsigned long crc;
} structPLTE;

typedef struct {
    char * data;
    unsigned int length;
    unsigned long crc;
} structIDAT;

typedef struct {
    unsigned long crc;
} structIEND;

typedef structPNG * PNG;
typedef structIHDR * IHDR;
typedef structPLTE * PLTE;
typedef structIDAT * IDAT;
typedef structIEND * IEND;

#define PNG_IMPL
#include "png.h"

bool is_png_header(char * header) {
    if(header == NULL || sizeof(header) != 8 ||\
                memcmp(header, PNG_HEADER, 8) != 0)
        return false;

    return true;
}

bool is_idhr_header(char * header) {
    if(header == NULL || sizeof(header) != 4 ||\
                memcmp(header, IHDR_HEADER, 4) != 0)
        return false;

    return true;
}

bool is_plte_header(char * header) {
    if(header == NULL || sizeof(header) != 4 ||\
                memcmp(header, PLTE_HEADER, 4) != 0)
        return false;
    
    return true;
}

bool is_idat_header(char * header) {
    if(header == NULL || sizeof(header) != 4 ||\
                memcmp(header, IDAT_HEADER, 4) != 0)
        return false;

    return true;
}

bool is_iend_header(char * header) {
    if(header == NULL || sizeof(header) != 4 ||\
                memcmp(header, IEND_HEADER, 4) != 0)
        return false;

    return true;
}

bool read_ihdr(PNG * png, FILE * file, int length) {
    if(length != 17) {
        perror("Invalid IHDR chunk length.");
        return false;
    }

    sIHDR ihdr = malloc(sizeof(structIHDR));
    MEM_CHECK(ihdr);

    for(int i = 3; i >= 0; i--) {
        ihdr->width = fgetc(file) << (8 * i);
        FEOF_CHECK(file);
    }

    for(int i = 3; i >= 0; i--) {
        ihdr->height = fgetc(file) << (8 * i);
        FEOF_CHECK(file);
    }

    if(width == 0 || height == 0 || width > 0x80000000 || height > 0x80000000) {
        perror("Invalid image dimensions.");
        return false;
    }
    
    ihdr->bit_depth = fgetc(file);
    FEOF_CHECK(file);
    
    ihdr->color_type = fgetc(file);
    FEOF_CHECK(file);

    if((ihdr->color_type == 0 && ihdr->bit_depth != 1 && ihdr->bit_depth != 2 &&\
        ihdr->bit_depth != 4 && ihdr->bit_depth != 8 && ihdr->bit_depth != 16) ||\
        (ihdr->color_type == 2 && ihdr->bit_depth != 8 && ihdr->bit_depth != 16) ||\
        (ihdr->color_type == 3 && ihdr->bit_depth != 1 && ihdr->bit_depth != 2 &&\
        ihdr->bit_depth != 4 && ihdr->bit_depth != 8) ||\
        (ihdr->color_type == 4 && ihdr->bit_depth != 8 && ihdr->bit_depth != 16) ||\
        (ihdr->color_type == 6 && ihdr->bit_depth != 8 && ihdr->bit_depth != 16)) {
        perror("Invalid bit depth and color type combination.");
        return false;
    }
    
    ihdr->compression_method = fgetc(file);
    FEOF_CHECK(file);

    if(ihdr->compression_method != 0) {
        perror("Invalid compression method.");
        return false;
    }
    
    ihdr->filter_method = fgetc(file);
    FEOF_CHECK(file);

    if(ihdr->filter_method != 0) {
        perror("Invalid filter method.");
        return false;
    }
    
    ihdr->interlace_method = fgetc(file);
    FEOF_CHECK(file);

    if(ihdr->interlace_method > 1) {
        perror("Invalid interlace method.");
        return false;
    }

    for(int i = 3; i >= 0; i--) {
        ihdr->crc = fgetc(file) << (8 * i);
        FEOF_CHECK(file);
    }

    (*png)->ihdr = ihdr;
    return true;
}

bool read_plte(PNG * png, FILE * file, int length) {
    if(length != 7) {
        perror("Invalid PLTE chunk length.");
        return false;
    }

    PLTE plte = malloc(sizeof(structPLTE));
    MEM_CHECK(plte);

    plte->red = fgetc(file);
    FEOF_CHECK(file);

    plte->green = fgetc(file);
    FEOF_CHECK(file);

    plte->blue = fgetc(file);
    FEOF_CHECK(file);

    for(int i = 3; i >= 0; i--) {
        plte->crc = fgetc(file) << (8 * i);
        FEOF_CHECK(file);
    }

    (*png)->plte = plte;
    return true;
}

bool read_idat(PNG * png, FILE * file, int length) {
    if(length < 4 || )

    IDAT idat = malloc(sizeof(structIDAT));
    MEM_CHECK(idat);

    idat->data = malloc(idat->length);
    MEM_CHECK(idat->data);

    for(int i = 0; i < idat->length; i++) {
        idat->data[i] = fgetc(file);
        FEOF_CHECK(file);
    }

    for(int i = 3; i >= 0; i--) {
        idat->crc = fgetc(file) << (8 * i);
        FEOF_CHECK(file);
    }
    
    (*png)->num_idat_chunks++;
    (*png)->idat = realloc((*png)->idat, (*png)->num_idat_chunks * sizeof(structIDAT));
    MEM_CHECK((*png)->idat);
    (*png)->idat[(*png)->num_idat_chunks - 1] = idat;

    return true;
}

bool read_iend(PNG * png, FILE * file) {
    IEND iend = malloc(sizeof(structIEND));
    MEM_CHECK(iend);

    for(int i = 3; i >= 0; i--) {
        iend->crc = fgetc(file) << (8 * i);
        FEOF_CHECK(file);
    }

    (*png)->iend = iend;
    return true;
}

PNG png_init() {
    PNG png = malloc(sizeof(structPNG));
    if(png == NULL)
        return NULL;
    
    png->filename = NULL;
    png->ihdr = NULL;
    png->plte = NULL;
    png->idat = NULL;
    png->iend = NULL;
    png->num_idat_chunks = 0;

    return png;
}

bool png_read_direct(PNG * png, FILE * file) {
    if(file == NULL)
        return false;
    
    char header[8];
    for(int i = 0; i < 8; i++) {
        header[i] = fgetc(file);
    }
    if(!is_png_header(header))
        return false;
    
    unsigned int chunk_size;
    char chunk_type[4];
    do {
        for(int i = 3; i >= 0; i--) {
            chunk_size = fgetc(file) << (8 * i);
            FEOF_CHECK(file);
        }

        for(int i = 0; i < 4; i++) {
            chunk_type[i] = fgetc(file);
            FEOF_CHECK(file);
        }
        
        if(is_idhr_header(chunk_type)) {
            if(!read_ihdr(png, file, chunk_size))
                return false;
        } else if(is_idat_header(chunkType)) {
            if(!read_idat(png, file, chunk_size))
                return false;
        }
    } while(!feof(file))
}

bool png_read_bitmap(PNG * png, FILE * file) {

}

plte_write_direct(PNG * png, FILE * file) {
    if((*png)->plte == NULL)
        return false;
    
    fprintf(file, "\x00\x00\x00\x07PLTE");
    fprintf(file, "%c%c%c", (*png)->plte->red, (*png)->plte->green, (*png)->plte->blue);
    fprintf(file, "%c%c%c%c", (*png)->plte->crc >> 24, (*png)->plte->crc >> 16, (*png)->plte->crc >> 8, (*png)->plte->crc);
}

bool png_write_direct(PNG * png, FILE * file) {
    fprintf(file, "\x89PNG\r\x0A\x1A\x0A");

    plte_write_direct(png, file);
}

bool png_write_bitmap(PNG * png, FILE * file) {

}

void png_free(PNG * png) {
    if(png == NULL)
        return;
    
    if((*png)->filename != NULL)
        free((*png)->filename);
    
    if((*png)->ihdr != NULL)
        free((*png)->ihdr);
    
    if((*png)->plte != NULL)
        free((*png)->plte);
    
    if((*png)->idat != NULL) {
        for(int i = 0; i < (*png)->num_idat_chunks; i++) {
            if((*png)->idat[i] != NULL) {
                if((*png)->idat[i]->data != NULL)
                    free((*png)->idat[i]->data);
                free((*png)->idat[i]);
            }
        }
        free((*png)->idat);
    }
    
    if((*png)->iend != NULL)
        free((*png)->iend);
    
    free(*png);
    *png = NULL;
}
