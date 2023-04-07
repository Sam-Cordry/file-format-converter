///
/// @file png.c
/// @brief PNG file format implementation
/// @author Sam Cordry

// include PNG header
#include "png.h"

#include "crc.h"

/// @brief The MEM_CHECK macro checks if the given pointer is NULL.
#define MEM_CHECK(ptr) if(ptr == NULL) { printf("Unable to allocate memory");\
                                            return false; }

/// @brief The FEOF_CHECK macro checks if the end of the file has been reached.
#define FEOF_CHECK(file) if(feof(file)) { printf("Unexpected end of file");\
                                            return false; }

/// @brief The is_png_header function checks if the string is a PNG header.
/// @param header The header to check.
/// @return True if the header is a PNG header, false otherwise.
bool is_png_header(const char* header) {
    return !(header == NULL || strlen(header) != 8 ||
                memcmp(header, PNG_HEADER, 8) != 0);
}

/// @brief The is_ihdr_header function checks if the string is an IHDR header.
/// @param header The header to check.
/// @return True if the header is an IHDR header, false otherwise.
bool is_idhr_header(const char* header) {
    return !(header == NULL || strlen(header) != 4 ||
                memcmp(header, IHDR_HEADER, 4) != 0);
}

/// @brief The is_plte_header function checks if the string is a PLTE header.
/// @param header The header to check.
/// @return True if the header is a PLTE header, false otherwise.
bool is_plte_header(const char* header) {
    return !(header == NULL || strlen(header) != 4 ||
                memcmp(header, PLTE_HEADER, 4) != 0);
}

/// @brief The is_idat_header function checks if the string is an IDAT header.
/// @param header The header to check.
/// @return True if the header is an IDAT header, false otherwise.
bool is_idat_header(const char* header) {
    return !(header == NULL || strlen(header) != 4 ||
                memcmp(header, IDAT_HEADER, 4) != 0);
}

/// @brief The is_iend_header function checks if the string is an IEND header.
/// @param header The header to check.
/// @return True if the header is an IEND header, false otherwise.
bool is_iend_header(const char* header) {
    return !(header == NULL || strlen(header) != 4 ||
                memcmp(header, IEND_HEADER, 4) != 0);
}

/// @brief The png_create function creates a PNG struct.
/// @return A pointer to the PNG struct.
PNG* png_create(void) {
    // allocate memory for the PNG struct
    PNG* png = malloc(sizeof(PNG));

    // check if the memory was allocated
    if(png == NULL)
        return NULL;
    
    // initialize the PNG struct
    png->ihdr = NULL;
    png->plte = NULL;
    png->idat = NULL;
    png->iend = NULL;
    png->num_idat_chunks = 0;

    // return the PNG struct
    return png;
}

/// @brief The png_destroy function destroys a PNG struct.
/// @param png The PNG struct to destroy.
/// @param file The file to close.
/// @param length The length of the IDAT chunk.
/// @return True if the PNG struct was destroyed, false otherwise.
bool read_ihdr(PNG* png, FILE* file, int length) {
    // check if the length is valid
    if(length != 13) {
        printf("Invalid IHDR chunk length");
        return false;
    }

    // allocate memory for the IHDR struct
    png->ihdr = malloc(sizeof(IHDR));
    MEM_CHECK(png->ihdr);

    // read the width
    int i;
    png->ihdr->width = 0;
    for(i = 0; i < 4; i++) {
        png->ihdr->width = png->ihdr->width | (fgetc(file) << (8 * (3 - i)));
        FEOF_CHECK(file);
    }

    // read the height
    for(i = 3; i >= 0; i--) {
        png->ihdr->height = fgetc(file) << (8 * i);
        FEOF_CHECK(file);
    }

    // check if the width and height are valid
    if(png->ihdr->width == 0 || png->ihdr->height == 0 ||
                    png->ihdr->width > 0x80000000 || png->ihdr->height > 0x80000000) {
        printf("Invalid image dimensions");
        return false;
    }
    
    // read the bit depth
    png->ihdr->bit_depth = fgetc(file);
    FEOF_CHECK(file);
    
    // read the color type
    png->ihdr->color_type = fgetc(file);
    FEOF_CHECK(file);

    // check if the bit depth and color type are a valid combination
    if((png->ihdr->color_type == 0 && png->ihdr->bit_depth != 1 &&
        png->ihdr->bit_depth != 2 && png->ihdr->bit_depth != 4 &&
        png->ihdr->bit_depth != 8 && png->ihdr->bit_depth != 16) ||
        (png->ihdr->color_type == 2 && png->ihdr->bit_depth != 8 &&
        png->ihdr->bit_depth != 16) || (png->ihdr->color_type == 3 &&
        png->ihdr->bit_depth != 1 && png->ihdr->bit_depth != 2 &&
        png->ihdr->bit_depth != 4 && png->ihdr->bit_depth != 8) ||
        (png->ihdr->color_type == 4 && png->ihdr->bit_depth != 8 &&
        png->ihdr->bit_depth != 16) || (png->ihdr->color_type == 6 &&
        png->ihdr->bit_depth != 8 && png->ihdr->bit_depth != 16)) {
        printf("Invalid bit depth and color type combination");
        return false;
    }
    
    // read the compression method
    png->ihdr->compression_method = fgetc(file);
    FEOF_CHECK(file);

    // check if the compression method is valid
    if(png->ihdr->compression_method != 0) {
        printf("Invalid compression method");
        return false;
    }
    
    // read the filter method
    png->ihdr->filter_method = fgetc(file);
    FEOF_CHECK(file);

    // check if the filter method is valid
    if(png->ihdr->filter_method != 0) {
        printf("Invalid filter method");
        return false;
    }
    
    // read the interlace method
    png->ihdr->interlace_method = fgetc(file);
    FEOF_CHECK(file);

    // check if the interlace method is valid
    if(png->ihdr->interlace_method > 1) {
        printf("Invalid interlace method");
        return false;
    }

    // read the CRC
    for(int i = 0; i < 4; i++) {
        png->ihdr->crc[i] = fgetc(file);
        FEOF_CHECK(file);
    }
    png->ihdr->crc[4] = '\0';

    unsigned char* data = malloc(17);
    memcpy(data, IHDR_HEADER, 4);
    memcpy(data + 4, png->ihdr->width, 4);
    memcpy(data + 8, png->ihdr->height, 4);
    memcpy(data + 12, png->ihdr->bit_depth, 1);
    memcpy(data + 13, png->ihdr->color_type, 1);
    memcpy(data + 14, png->ihdr->compression_method, 1);
    memcpy(data + 15, png->ihdr->filter_method, 1);
    memcpy(data + 16, png->ihdr->interlace_method, 1);
    unsigned char* calc_crc = (unsigned char*) &crc(data, 17);

    for(int i = 0; i < 4; i++) {
        if(png->ihdr->crc[i] != calc_crc[3 - i]) {
            printf("Invalid PNg: Failed CRC Check");
            return false;
        }
    }

    return true;
}

/// @brief The read_plte function reads a PLTE chunk.
/// @param png The PNG struct to read the PLTE chunk into.
/// @param file The file to read the PLTE chunk from.
/// @param length The length of the PLTE chunk.
/// @return True if the PLTE chunk was read, false otherwise.
bool read_plte(PNG* png, FILE* file, int length) {
    // check if the length is valid
    if(length != 3) {
        printf("Invalid PLTE chunk length");
        return false;
    }

    // allocate memory for the PLTE struct
    png->plte = malloc(sizeof(PLTE));
    MEM_CHECK(png->plte);

    // read the red value
    png->plte->red = fgetc(file);
    FEOF_CHECK(file);

    // read the green value
    png->plte->green = fgetc(file);
    FEOF_CHECK(file);

    // read the blue value
    png->plte->blue = fgetc(file);
    FEOF_CHECK(file);

    // read the CRC
    for(int i = 0; i < 4; i++) {
        png->plte->crc[i] = fgetc(file);
        FEOF_CHECK(file);
    }
    png->plte->crc[4] = '\0';

    unsigned char* data = malloc(7);
    memcpy(data, PLTE_HEADER, 4);
    memcpy(data + 4, png->plte->red, 1);
    memcpy(data + 5, png->plte->green, 1);
    memcpy(data + 6, png->plte->blue, 1);
    unsigned char* calc_crc = (unsigned char*) &crc(data, 7);

    for(int i = 0; i < 4; i++) {
        if(png->plte->crc[i] != calc_crc[3 - i]) {
            printf("Invalid PNG: Failed CRC Check");
            return false;
        }
    }

    return true;
}

/// @brief The read_idat function reads an IDAT chunk.
/// @param png The PNG struct to read into.
/// @param file The file to read from.
/// @param length The length of the IDAT chunk.
/// @return True if the IDAT chunk was read, false otherwise.
bool read_idat(PNG* png, FILE* file, int length) {
    // increment the number of IDAT chunks and allocate memory appropriately
    if(png->num_idat_chunks == 0) {
        png->num_idat_chunks = 1;
        png->idat = malloc(sizeof(IDAT));
    } else {
        png->num_idat_chunks++;
        png->idat = realloc(png->idat, png->num_idat_chunks* sizeof(IDAT));
    }
    MEM_CHECK(png->idat);

    // allocate memory for the IDAT struct
    png->idat[png->num_idat_chunks - 1].data = malloc(length);
    MEM_CHECK(png->idat->data);

    // set the length of the IDAT chunk
    png->idat[png->num_idat_chunks - 1].length = length;

    // read the data
    for(int i = 0; i < length; i++) {
        png->idat->data[i] = fgetc(file);
        FEOF_CHECK(file);
    }

    // read the CRC
    for(int i = 0; i < 4; i++) {
        png->idat->crc[i] = fgetc(file);
        FEOF_CHECK(file);
    }
    png->idat->crc[4] = '\0';

    unsigned char* data = malloc(length + 4);
    memcpy(data, &IDAT_HEADER, 4);
    memcpy(data + 4, png->idat->data, length);
    unsigned char* calc_crc = (unsigned char*) &crc(data, length = 4);

    for(int i = 0; i < 4; i++) {
        if(calc_crc[3 - i] != png->idat->crc[i]) {
            printf("Invalid PNG: Failed CRC Check");
            return false;
        }
    }

    return true;
}

/// @brief The read_iend function reads an IEND chunk.
/// @param png The PNG struct to read into.
/// @param file The file to read from.
/// @return True if the IEND chunk was read, false otherwise.
bool read_iend(PNG* png, FILE* file) {
    // allocate memory for the IEND struct
    png->iend = malloc(sizeof(IEND));
    MEM_CHECK(png->iend);

    // read the CRC
    for(int i = 0; i < 4; i++) {
        png->iend->crc[i] = fgetc(file);
        FEOF_CHECK(file);
    }
    png->iend->crc[4] = '\0';
    
    if(strcmp(png->iend->crc, "\xAE\x42\x60\x82") != 0) {
        printf("Invalid PNG: Failed CRC Check");
        return false;
    }

    return true;
}

/// @brief The png_read function reads a PNG file from a given file.
/// @param png The PNG struct to read into.
/// @param file The file to read from.
/// @return True if the PNG file was read, false otherwise.
bool png_read(PNG* png, FILE* file) {
    // check if the PNG and file exist
    if(file == NULL || png == NULL)
        return false;
    
    // read the header
    char header[9];
    for(int i = 0; i < 8; i++) {
        header[i] = fgetc(file);
    }
    header[8] = '\0';

    // check if the header is valid
    if(!is_png_header(header))
        return false;

    // read the chunks while there are still chunks to read
    unsigned int chunk_size;
    char chunk_type[5];
    chunk_type[4] = '\0';
    while(!feof(file)) {
        // read the chunk size
        for(int i = 3; i >= 0; i--) {
            chunk_size = fgetc(file) << (8* i);
            FEOF_CHECK(file);
        }

        // read the chunk type
        for(int i = 0; i < 4; i++) {
            chunk_type[i] = fgetc(file);
            FEOF_CHECK(file);
        }
        
        // find the chunk type and read it accordingly
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
    }
    return true;
}

/// @brief The ihdr_write function writes an IHDR chunk to a file.
/// @param ihdr The IHDR struct to write from.
/// @param file The file to write to.
/// @return True if the IHDR chunk was written, false otherwise.
bool ihdr_write(IHDR* ihdr, FILE* file) {
    // check if the IHDR chunk exists
    if(ihdr == NULL)
        return false;
    
    // write the IHDR header
    char zero = 0;
    fprintf(file, "%c%c%c\x0D%s", zero, zero, zero, IHDR_HEADER);

    // write the width
    int i;
    for(i = 0; i < 4; i++)
        fprintf(file, "%c", ((char) ((ihdr->width >> (8* (3 - i)))) & 0xFF));
    
    // write the height
    for(i = 0; i < 4; i++)
        fprintf(file, "%c", ((char) ((ihdr->height >> (8* (3 - i)))) & 0xFF));
    
    // write the other data present in the IHDR chunk
    fprintf(file, "%c", ihdr->bit_depth);
    fprintf(file, "%c", ihdr->color_type);
    fprintf(file, "%c", ihdr->compression_method);
    fprintf(file, "%c", ihdr->filter_method);
    fprintf(file, "%c", ihdr->interlace_method);
    fprintf(file, "%s", ihdr->crc);

    return true;
}

/// @brief The plte_write function writes a PLTE chunk to a file.
/// @param plte The PLTE struct to write from. 
/// @param file The file to write to.
/// @return True if the PLTE chunk was written, false otherwise.
bool plte_write(PLTE* plte, FILE* file) {
    // check if the PLTE chunk exists
    if(plte == NULL)
        return false;
    
    // write the PLTE header
    char zero = 0;
    fprintf(file, "%c%c%cx07%s", zero, zero, zero, PLTE_HEADER);

    // write the palette
    fprintf(file, "%c%c%c", plte->red, plte->green, plte->blue);

    // write the CRC
    fprintf(file, "%s", plte->crc);

    return true;
}

/// @brief The idat_write function writes all IDAT chunks to a file.
/// @param png The PNG struct to write from.
/// @param file The file to write to.
/// @return True if the IDAT chunk was written, false otherwise.
bool idat_write(PNG* png, FILE* file) {
    // check if the IDAT chunk exists
    if(png->idat == NULL)
        return false;
    
    // loop for every IDAT chunk
    char zero = 0;
    for(unsigned int i = 0; i < png->num_idat_chunks; i++) {
        // write the length
        unsigned int j;
        for(j = 0; j < 4; j++)
            fprintf(file, "%c", ((int) (png->idat[i].length / pow(256, 3 - j))) % 256);
        
        // write the IDAT header
        fprintf(file, "%s", IDAT_HEADER);

        // write the data
        for(j = 0; j < png->idat[i].length; j++)
            fprintf(file, "%c", (png->idat[i].data[j] == '\0') ? zero : png->idat[i].data[j]);
        
        // write the CRC
        fprintf(file, "%s", png->idat[i].crc);
    }

    return true;
}

/// @brief The iend_write function writes an IEND chunk to a file.
/// @param iend The IEND struct to write from.
/// @param file The file to write to.
/// @return True if the IEND chunk was written, false otherwise.
bool iend_write(IEND* iend, FILE* file) {
    // check if the IEND chunk exists
    if(iend == NULL)
        return false;
    
    // write the IEND header
    char zero = 0;
    fprintf(file, "%c%c%c%c%s", zero, zero, zero, zero, IEND_HEADER);

    // write the CRC
    fprintf(file, "%s", iend->crc);

    return true;
}

/// @brief The png_write function writes a PNG struct to a file.
/// @param png The PNG struct to write from.
/// @param file The file to write to.
/// @return True if the PNG struct was written, false otherwise.
bool png_write(PNG* png, FILE* file) {
    // write the PNG header
    fprintf(file, "%s", PNG_HEADER);

    // write the chunks in the correct order
    ihdr_write(png->ihdr, file);
    plte_write(png->plte, file);
    idat_write(png, file);
    iend_write(png->iend, file);

    return true;
}

/// @brief The png_free function frees the memory allocated to a PNG struct.
/// @param png The PNG struct to free.
void png_free(PNG* png) {
    // check if the PNG struct exists
    if(png == NULL)
        return;

    // free the IHDR chunk if it exists
    if(png->ihdr != NULL)
        free(png->ihdr);
    
    // free the PLTE chunk if it exists
    if(png->plte != NULL)
        free(png->plte);
    
    // free the IDAT chunks if they exist
    if(png->idat != NULL) {
        for(unsigned int i = 0; i < png->num_idat_chunks; i++) {
            if(png->idat[i].data != NULL)
                free(png->idat[i].data);
        }
        free(png->idat);
    }
    
    // free the IEND chunk if it exists
    if(png->iend != NULL)
        free(png->iend);

    // free the PNG struct    
    free(png);
}
