#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char * filename;
    IHDR * ihdr;
    PLTE * plte;
    IDAT * idat;
    IEND * iend;
    unsigned int plte_length;
    unsigned int idat_length;
} structPNG;

typedef structPNG * PNG;

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int bit_depth;
    char color_type;
    char compression_method;
    char filter_method;
    char interlace_method;
} structIHDR;

typedef structIHDR * IHDR;

typedef struct {
    char red;
    char green;
    char blue;
} structPLTE;

typedef structPLTE * PLTE;

typedef struct {
    char * data;
    unsigned int length;
} structIDAT;

#define PNG_IMPL
#define IHDR_IMPL
#define PLTE_IMPL
#define IDAT_IMPL
#define IEND_IMPL
#include "png.h"

bool is_png_header(char * header) {
    if(header == NULL || sizeof(header) != 8)
        return false;
    
    for(int i = 0; i < 8; i++)
        if(header[i] != PNG_HEADER[i])
            return false;

    return true;
}

bool is_idhr_header(char * header) {
    if(header == NULL || sizeof(header) != 4)
        return false;
    
    for(int i = 0; i < 4; i++)
        if(header[i] != IDHR_HEADER[i])
            return false;

    return true;
}

bool is_idat_header(char * header) {
    if(header == NULL || sizeof(header) != 4)
        return false;
    
    for(int i = 0; i < 4; i++)
        if(header[i] != IDAT_HEADER[i])
            return false;

    return true;
}

bool is_iend_header(char * header) {
    if(header == NULL || sizeof(header) != 4)
        return false;
    
    for(int i = 0; i < 4; i++)
        if(header[i] != IEND_HEADER[i])
            return false;

    return true;
}

bool read_ihdr(PNG * png, FILE * file) {
    IHDR ihdr = malloc(sizeof(structIHDR));
    
}

PNG init_png() {
    PNG png = malloc(sizeof(structPNG));
    if(png == NULL)
        return NULL;
    
    png->filename = NULL;
    png->ihdr = NULL;
    png->plte = NULL;
    png->idat = NULL;
    png->iend = NULL;

    return png;
}

bool read_png_direct(PNG * png, FILE * file) {
    if(file == NULL)
        return false;
    
    char header[8];
    for(int i = 0; i < 8; i++) {
        header[i] = fgetc(file);
    }
    if(!is_png_header(header))
        return false;
    
    unsigned int chunkSize;
    char chunkType[4];
    unsigned long crcLong = 0x00000000;
    do {
        chunkSize = fgetc(file) << 24 | fgetc(file) << 16 | fgetc(file) << 8 | fgetc(file);
        unsigned char chunkData[chunkSize + 5];

        for(int i = 0; i < 4; i++) {
            chunkType[i] = fgetc(file);
            chunkData[i] = chunkType[i];
        }

        for(int i = 0; i < chunkSize; i++) {
            chunkData[i + 4] = fgetc(file);
        }
        chunkData[chunkSize + 4] = '\0';
        
        crcLong = fgetc(file) << 24 | fgetc(file) << 16 | fgetc(file) << 8 | fgetc(file);
        crcLong &= 0x00000000FFFFFFFF;
        if(crcLong != crc32(chunkData, chunkSize + 4))
            return false;
        
        if(is_idhr_header(chunkType)) {
            (*png)->width = chunkData[0] << 24 | chunkData[1] << 16 | chunkData[2] << 8 | chunkData[3];
            (*png)->height = chunkData[4] << 24 | chunkData[5] << 16 | chunkData[6] << 8 | chunkData[7];
            (*png)->bit_depth = chunkData[8];
            (*png)->color_type = chunkData[9];
            (*png)->compression_method = chunkData[10];
            (*png)->filter_method = chunkData[11];
            (*png)->interlace_method = chunkData[12];
        } else if(is_idat_header(chunkType)) {
            (*png)->data = malloc(chunkSize);
            if((*png)->data == NULL)
                return false;
            for(int i = 0; i < chunkSize; i++) {
                (*png)->data[i] = chunkData[i + 4];
            }
        }
    }
}

// int main() {
//     char filename[81];
//     printf("Enter the filename: ");
//     scanf("%s", filename);
//     FILE *file = fopen(filename, "r");

//     if (file == NULL) {
//         printf("Error opening file");
//         return 1;
//     }
    
//     char header[8];
//     for(int i = 0; i < 8; i++) {
//         header[i] = fgetc(file);
//     }
//     printf("Is file a PNG? ");
//     if(!isPNGHeader(header)) {
//         printf("No\n");
//         return 1;
//     }
//     printf("Yes\n");

//     unsigned int chunkSize;
//     char chunkType[4];
//     unsigned long crcLong = 0x00000000;
//     do {
//         chunkSize = fgetc(file) << 24 | fgetc(file) << 16 | fgetc(file) << 8 | fgetc(file);
//         unsigned char chunkData[chunkSize + 5];

//         for(int i = 0; i < 4; i++) {
//             chunkType[i] = fgetc(file);
//             chunkData[i] = chunkType[i];
//         }

//         for(int i = 0; i < chunkSize; i++) {
//             chunkData[i + 4] = fgetc(file);
//         }
//         chunkData[chunkSize + 4] = '\0';
        
//         crcLong = fgetc(file) << 24 | fgetc(file) << 16 | fgetc(file) << 8 | fgetc(file);
//         crcLong &= 0x00000000FFFFFFFF;
//         if(crcLong != crc(chunkData, chunkSize + 4)) {
//             printf("CRC Error! Invalid PNG!\n");
//             printf("Current Chunk Data: %s\n", chunkData);
//             printf("Generated CRC Code: %lX\n", crc(chunkData, chunkSize + 4));
//             printf("Read CRC Code: %lX\n", crcLong);
//             return 1;
//         }

//         if(isIHDRHeader(chunkType)) {
//             width = chunkData[4] << 24 | chunkData[5] << 16 | chunkData[6] << 8 | chunkData[7];
//             height = chunkData[8] << 24 | chunkData[9] << 16 | chunkData[10] << 8 | chunkData[11];
//             bitDepth = chunkData[12];
//             colorType = chunkData[13];
//             compressionMethod = chunkData[14];
//             filterMethod = chunkData[15];
//             interlaceMethod = chunkData[16];
//         } else if(isIDATHeader(chunkType)) {

//         }
//     } while(!isIENDHeader(chunkType));

//     crcLong = fgetc(file) << 24 | fgetc(file) << 16 | fgetc(file) << 8 | fgetc(file);
//     crcLong &= 0x00000000FFFFFFFF;
//     if(crcLong != crc(iendHeader, 4)) {
//         printf("CRC Error! Invalid PNG!\n");
//         return 1;
//     }

//     printf("PNG Processed Successfully!\n");

//     fclose(file);
//     return 0;
// }
