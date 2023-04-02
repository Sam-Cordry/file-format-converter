#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MEM_CHECK(ptr) if(ptr == NULL) { printf("Unable to allocate memory");\
                                            return false; }
#define FEOF_CHECK(file) if(feof(file)) { printf("Unexpected end of file");\
                                            return false; }

#include "png.h"

bool is_png_header(const char* header) {
    return !(header == NULL || strlen(header) != 8 ||
                memcmp(header, PNG_HEADER, 8) != 0);
}

bool is_idhr_header(const char* header) {
    return !(header == NULL || strlen(header) != 4 ||
                memcmp(header, IHDR_HEADER, 4) != 0);
}

bool is_plte_header(const char* header) {
    return !(header == NULL || strlen(header) != 4 ||
                memcmp(header, PLTE_HEADER, 4) != 0);
}

bool is_idat_header(const char* header) {
    return !(header == NULL || strlen(header) != 4 ||
                memcmp(header, IDAT_HEADER, 4) != 0);
}

bool is_iend_header(const char* header) {
    return !(header == NULL || strlen(header) != 4 ||
                memcmp(header, IEND_HEADER, 4) != 0);
}

bool read_ihdr(PNG* png, FILE* file, int length) {
    if(length != 13) {
        printf("Invalid IHDR chunk length");
        return false;
    }

    png->ihdr = malloc(sizeof(IHDR));
    MEM_CHECK(png->ihdr);

    IHDR * ihdr = png->ihdr;

    ihdr->width = 0;

    int i;
    for(i = 0; i < 4; i++) {
        ihdr->width = ihdr->width | (fgetc(file) << (8 * (3 - i)));
        FEOF_CHECK(file);
    }

    for(i = 3; i >= 0; i--) {
        ihdr->height = fgetc(file) << (8 * i);
        FEOF_CHECK(file);
    }

    if(ihdr->width == 0 || ihdr->height == 0 ||
                    ihdr->width > 0x80000000 || ihdr->height > 0x80000000) {
        printf("Invalid image dimensions");
        return false;
    }
    
    ihdr->bit_depth = fgetc(file);
    FEOF_CHECK(file);
    
    ihdr->color_type = fgetc(file);
    FEOF_CHECK(file);

    if((ihdr->color_type == 0 && ihdr->bit_depth != 1 && ihdr->bit_depth != 2 &&
        ihdr->bit_depth != 4 && ihdr->bit_depth != 8 && ihdr->bit_depth != 16) ||
        (ihdr->color_type == 2 && ihdr->bit_depth != 8 && ihdr->bit_depth != 16) ||
        (ihdr->color_type == 3 && ihdr->bit_depth != 1 && ihdr->bit_depth != 2 &&
        ihdr->bit_depth != 4 && ihdr->bit_depth != 8) ||
        (ihdr->color_type == 4 && ihdr->bit_depth != 8 && ihdr->bit_depth != 16) ||
        (ihdr->color_type == 6 && ihdr->bit_depth != 8 && ihdr->bit_depth != 16)) {
        printf("Invalid bit depth and color type combination");
        return false;
    }
    
    ihdr->compression_method = fgetc(file);
    FEOF_CHECK(file);

    if(ihdr->compression_method != 0) {
        printf("Invalid compression method");
        return false;
    }
    
    ihdr->filter_method = fgetc(file);
    FEOF_CHECK(file);

    if(ihdr->filter_method != 0) {
        printf("Invalid filter method");
        return false;
    }
    
    ihdr->interlace_method = fgetc(file);
    FEOF_CHECK(file);

    if(ihdr->interlace_method > 1) {
        printf("Invalid interlace method");
        return false;
    }

    for(int i = 0; i < 4; i++) {
        ihdr->crc[i] = fgetc(file);
        FEOF_CHECK(file);
    }
    ihdr->crc[4] = '\0';

    return true;
}

bool read_plte(PNG * png, FILE * file, int length) {
    if(length != 3) {
        printf("Invalid PLTE chunk length");
        return false;
    }

    png->plte = malloc(sizeof(PLTE));
    MEM_CHECK(png->plte);

    PLTE * plte = png->plte;

    plte->red = fgetc(file);
    FEOF_CHECK(file);

    plte->green = fgetc(file);
    FEOF_CHECK(file);

    plte->blue = fgetc(file);
    FEOF_CHECK(file);

    for(int i = 0; i < 4; i++) {
        plte->crc[i] = fgetc(file);
        FEOF_CHECK(file);
    }
    plte->crc[4] = '\0';

    return true;
}

bool read_idat(PNG * png, FILE * file, int length) {
    if(png->num_idat_chunks == 0) {
        png->num_idat_chunks = 1;
        png->idat = malloc(sizeof(IDAT));
    } else {
        png->num_idat_chunks++;
        png->idat = realloc(png->idat, png->num_idat_chunks * sizeof(IDAT));
    }
    MEM_CHECK(png->idat);

    IDAT * idat = &png->idat[png->num_idat_chunks - 1];

    idat->length = length;

    idat->data = malloc(length);
    MEM_CHECK(idat->data);

    for(int i = 0; i < length; i++) {
        idat->data[i] = fgetc(file);
        FEOF_CHECK(file);
    }

    for(int i = 0; i < 4; i++) {
        idat->crc[i] = fgetc(file);
        FEOF_CHECK(file);
    }
    idat->crc[4] = '\0';

    return true;
}

bool read_iend(PNG * png, FILE * file) {
    png->iend = malloc(sizeof(IEND));
    MEM_CHECK(png->iend);

    for(int i = 0; i < 4; i++) {
        png->iend->crc[i] = fgetc(file);
        FEOF_CHECK(file);
    }
    png->iend->crc[4] = '\0';

    return true;
}

PNG * png_create() {
    PNG * png = malloc(sizeof(PNG));
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

bool png_read(PNG * png, FILE * file) {
    if(file == NULL || png == NULL)
        return false;
    
    char header[9];
    for(int i = 0; i < 8; i++) {
        header[i] = fgetc(file);
    }
    header[8] = '\0';
    if(!is_png_header(header))
        return false;

    unsigned int chunk_size;
    char chunk_type[5];
    chunk_type[4] = '\0';
    do {
        for(int i = 3; i >= 0; i--) {
            chunk_size = fgetc(file) << (8 * i);
            FEOF_CHECK(file);
        }

        for(int i = 0; i < 4; i++) {
            chunk_type[i] = fgetc(file);
            FEOF_CHECK(file);
        }

        // TODO: Print these if verbose
        // printf("Chunk size: %d\n", chunk_size);
        // printf("Chunk type: %s\n", chunk_type);
        
        if(is_idhr_header(chunk_type)) {
            if(!read_ihdr(png, file, chunk_size))
                return false;
        } else if(is_idat_header(chunk_type)) {
            if(!read_idat(png, file, chunk_size))
                return false;
        } else if(is_iend_header(chunk_type)) {
            if(!read_iend(png, file))
                return false;
            else
                break;
        } else {
            printf("Not recognized.\n\n");
        }
    } while(!feof(file));
    return true;
}

bool ihdr_write_direct(PNG * png, FILE * file) {
    if(png->ihdr == NULL)
        return false;
    
    char zero = 0;
    fprintf(file, "%c%c%c\x0D%s", zero, zero, zero, IHDR_HEADER);

    IHDR * ihdr = png->ihdr;

    int i;
    for(i = 0; i < 4; i++)
        fprintf(file, "%c", ((char) ((ihdr->width >> (8 * (3 - i)))) & 0xFF));
    for(i = 0; i < 4; i++)
        fprintf(file, "%c", ((char) ((ihdr->height >> (8 * (3 - i)))) & 0xFF));
    fprintf(file, "%c", ihdr->bit_depth);
    fprintf(file, "%c", ihdr->color_type);
    fprintf(file, "%c", ihdr->compression_method);
    fprintf(file, "%c", ihdr->filter_method);
    fprintf(file, "%c", ihdr->interlace_method);
    fprintf(file, "%s", ihdr->crc);
    return true;
}

bool plte_write_direct(PNG * png, FILE * file) {
    if(png->plte == NULL)
        return false;
    
    char zero = 0;
    fprintf(file, "%c%c%cx07%s", zero, zero, zero, PLTE_HEADER);
    fprintf(file, "%c%c%c", png->plte->red, png->plte->green, png->plte->blue);
    fprintf(file, "%s", png->plte->crc);
    return true;
}

bool idat_write_direct(PNG * png, FILE * file) {
    if(png->idat == NULL)
        return false;
    
    char zero = 0;
    for(unsigned int i = 0; i < png->num_idat_chunks; i++) {
        unsigned int j;
        for(j = 0; j < 4; j++)
            fprintf(file, "%c", ((int) (png->idat[i].length / pow(256, 3 - j))) % 256);
        fprintf(file, "%s", IDAT_HEADER);
        for(j = 0; j < png->idat[i].length; j++)
            fprintf(file, "%c", (png->idat[i].data[j] == '\0') ? zero : png->idat[i].data[j]);
        fprintf(file, "%s", png->idat[i].crc);
    }
    return true;
}

bool iend_write_direct(PNG * png, FILE * file) {
    if(png->iend == NULL)
        return false;
    
    char zero = 0;
    fprintf(file, "%c%c%c%c%s", zero, zero, zero, zero, IEND_HEADER);
    fprintf(file, "%s", png->iend->crc);
    return true;
}

bool png_write(PNG * png, FILE * file) {
    fprintf(file, "%s", PNG_HEADER);

    ihdr_write_direct(png, file);
    plte_write_direct(png, file);
    idat_write_direct(png, file);
    iend_write_direct(png, file);

    fflush(file);

    return true;
}

void png_free(PNG * png) {
    if(png == NULL)
        return;
    
    if(png->filename != NULL)
        free(png->filename);

    
    if(png->ihdr != NULL)
        free(png->ihdr);
    
    if(png->plte != NULL)
        free(png->plte);
    
    if(png->idat != NULL) {
        for(unsigned int i = 0; i < png->num_idat_chunks; i++) {
            if(png->idat[i].data != NULL)
                free(png->idat[i].data);
        }
        free(png->idat);
    }
    
    if(png->iend != NULL)
        free(png->iend);
    
    free(png);
}
