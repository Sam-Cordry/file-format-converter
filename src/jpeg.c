///
/// @file jpeg.c
/// @brief JPEG file format implementation
/// @author Sam Cordry

// include the header for the JPEG file format
#include "jpeg.h"

/// @brief The MEM_CHECK macro checks if the given pointer is NULL.
#define MEM_CHECK(ptr) if(ptr == NULL) { printf("Unable to allocate memory");\
                                            return false; }

/// @brief The FEOF_CHECK macro checks if the end of the file has been reached.
#define FEOF_CHECK(file) if(feof(file)) { printf("Unexpected end of file");\
                                            return false; }

#ifdef DEBUG
/// @brief The print_info function prints the given data to the console.
/// @param data The data to print.
/// @param length The length of the data.
void print_info(unsigned char* data, int length) {
    printf("Ran with length: %d\nData:\n", length);
    for(int i = 0; i < length; i++) {
        printf("%02X", data[i]);
        if(i % 2 == 1)
            printf(" ");
    }
    printf("\n\n");
}
#endif

/// @brief The jpeg_create function initializes a pointer to a JPEG struct.
/// @return A pointer to the created JPEG struct.
JPEG* jpeg_create(void) {
    JPEG* jpeg = malloc(sizeof(JPEG));
    
    jpeg->frames = NULL;
    jpeg->quant_tables = NULL;
    jpeg->huff_tables = NULL;
    jpeg->scans = NULL;
    jpeg->app_segments = NULL;

    jpeg->num_frames = 0;
    jpeg->num_quant_tables = 0;
    jpeg->num_huff_tables = 0;
    jpeg->num_scans = 0;
    jpeg->num_app_segments = 0;

    return jpeg;
}

/// @brief The jpeg_read function reads a frame segment from the given data.
/// @param jpeg The JPEG struct to read into.
/// @param data The data to read from.
/// @param length The length of the data.
/// @return True if the frame was read successfully, false otherwise.
bool jpeg_read_frame(JPEG* jpeg, unsigned char* data, int length) {
    // terate the number of frames and allocate memory appropriately
    if(jpeg->num_frames == 0) {
        jpeg->num_frames = 1;
        jpeg->frames = malloc(sizeof(FRAME));
    } else {
        jpeg->num_frames++;
        jpeg->frames = realloc(jpeg->frames, sizeof(FRAME) * jpeg->num_frames);
    }
    MEM_CHECK(jpeg->frames);

    // allocate memory for the frame data
    jpeg->frames[jpeg->num_frames - 1].data = malloc(length);
    MEM_CHECK(jpeg->frames[jpeg->num_frames - 1].data);

    // set the frame data length
    jpeg->frames[jpeg->num_frames - 1].length = length;

    // copy the data into the frame data
    memcpy(jpeg->frames[jpeg->num_frames - 1].data, data, length);

    return true;
}

/// @brief The jpeg_read_quant_table function reads a quantization table
///        segment from the given data.
/// @param jpeg The JPEG struct to read into.
/// @param data  The data to read from.
/// @param length The length of the data.
/// @return True if the table was read successfully, false otherwise.
bool jpeg_read_quant_table(JPEG* jpeg, unsigned char* data, int length) {
    // iterate the number of tables and allocate memory appropriately
    if(jpeg->num_quant_tables == 0) {
        jpeg->num_quant_tables = 1;
        jpeg->quant_tables = malloc(sizeof(QUANT_TABLE));
    } else {
        jpeg->quant_tables = realloc(jpeg->quant_tables,\
                                sizeof(QUANT_TABLE) * jpeg->num_quant_tables);
        jpeg->num_quant_tables++;
    }
    MEM_CHECK(jpeg->quant_tables);

    // allocate memory for the table data
    jpeg->quant_tables[jpeg->num_quant_tables - 1].data = malloc(length);
    MEM_CHECK(jpeg->quant_tables[jpeg->num_quant_tables - 1].data);

    // set the table data length
    jpeg->quant_tables[jpeg->num_quant_tables - 1].length = length;

    // copy the data into the table data
    memcpy(jpeg->quant_tables[jpeg->num_quant_tables - 1].data, data, length);

    return true;
}

/// @brief The jpeg_read_huff_table function reads a huffman table segment
///        from the given data.
/// @param jpeg The JPEG struct to read into.
/// @param data The data to read from.
/// @param length The length of the data.
/// @return True if the table was read successfully, false otherwise.
bool jpeg_read_huff_table(JPEG* jpeg, unsigned char* data, int length) {
    // iterate the number of tables and allocate memory appropriately
    if(jpeg->num_huff_tables == 0) {
        jpeg->num_huff_tables = 1;
        jpeg->huff_tables = malloc(sizeof(HUFF_TABLE));
    } else {
        jpeg->num_huff_tables++;
        jpeg->huff_tables = realloc(jpeg->huff_tables,\
                                sizeof(HUFF_TABLE) * jpeg->num_huff_tables);
    }
    MEM_CHECK(jpeg->huff_tables);

    // allocate memory for the table data
    jpeg->huff_tables[jpeg->num_huff_tables - 1].data = malloc(length);
    MEM_CHECK(jpeg->huff_tables[jpeg->num_huff_tables - 1].data);

    // set the table data length
    jpeg->huff_tables[jpeg->num_huff_tables - 1].length = length;

    // copy the data into the table data
    memcpy(jpeg->huff_tables[jpeg->num_huff_tables - 1].data, data, length);

    return true;
}

/// @brief The jpeg_read_scan function reads a scan segment from the given data.
/// @param jpeg The JPEG struct to read into.
/// @param data The data to read from.
/// @param length The length of the data.
/// @return True if the scan was read successfully, false otherwise.
bool jpeg_read_scan(JPEG* jpeg, unsigned char* data, int length) {
    // iterate the number of scans and allocate memory appropriately
    if(jpeg->num_scans == 0) {
        jpeg->num_scans = 1;
        jpeg->scans = malloc(sizeof(SCAN));
    } else {
        jpeg->num_scans++;
        jpeg->scans = realloc(jpeg->scans, sizeof(SCAN) * jpeg->num_scans);
    }
    MEM_CHECK(jpeg->scans);

    // allocate memory for the scan data
    jpeg->scans[jpeg->num_scans - 1].data = malloc(length);
    MEM_CHECK(jpeg->scans[jpeg->num_scans - 1].data);

    // set the scan data length
    jpeg->scans[jpeg->num_scans - 1].length = length;

    // copy the data into the scan datas
    memcpy(jpeg->scans[jpeg->num_scans - 1].data, data, length);

    return true;
}

/// @brief The jpeg_read_app_seg function reads an application segment from the
///        given data.
/// @param jpeg The JPEG struct to read into.
/// @param data The data to read from.
/// @param length The length of the data.
/// @return True if the segment was read successfully, false otherwise.
bool jpeg_read_app_seg(JPEG* jpeg, unsigned char* data, int length) {
    // iterate the number of app segments and allocate memory appropriately
    if(jpeg->num_app_segments == 0) {
        jpeg->num_app_segments = 1;
        jpeg->app_segments = malloc(sizeof(APP_SEG));
    } else {
        jpeg->num_app_segments++;
        jpeg->app_segments = realloc(jpeg->app_segments,\
                                sizeof(APP_SEG) * jpeg->num_app_segments);
    }
    MEM_CHECK(jpeg->app_segments);

    // allocate memory for the app segment data
    jpeg->app_segments[jpeg->num_app_segments - 1].data = malloc(length);
    MEM_CHECK(jpeg->app_segments[jpeg->num_app_segments - 1].data);

    // set the app segment data length
    jpeg->app_segments[jpeg->num_app_segments - 1].length = length;

    // copy the data into the app segment data
    memcpy(jpeg->app_segments[jpeg->num_app_segments - 1].data, data, length);

    return true;
}

/// @brief The jpeg_read function reads a JPEG file from the given file.
/// @param jpeg The JPEG struct to read into.
/// @param file The file to read from.
/// @return True if the file was read successfully, false otherwise.
bool jpeg_read(JPEG* jpeg, FILE* file) {
    // check if the file starts with the JPEG start marker
    if(fgetc(file) != START) {
        printf("Invalid JPEG file\n");
        return false;
    }

    // set variables for later use
    unsigned char marker;
    unsigned char* data = malloc(MAX_DATA_LENGTH + 1);

    // read the file until the end of the file is reached
    while(!feof(file)) {
        // read the marker
        marker = fgetc(file);
        FEOF_CHECK(file);
        
        // read the data
        char c;
        int i;
        for(i = 0; (unsigned char) (c = fgetc(file)) != START; i++) {
            data[i] = (unsigned char) c;
            FEOF_CHECK(file);
        }
        data[i] = '\0';

        // read the data based on the marker, if the marker is recognized
        switch(marker) {
            case SOF0:
            case SOF1:
            case SOF2:
            case SOF3:
            case SOF5:
            case SOF6:
            case SOF7:
            case SOF9:
            case SOF10:
            case SOF11:
            case SOF13:
            case SOF14:
            case SOF15:
                // read data as a frame
                jpeg_read_frame(jpeg, data, i);
                break;
            case DQT:
                // read data as a quantization table
                jpeg_read_quant_table(jpeg, data, i);
                break;
            case DHT:
                // read data as a huffman table
                jpeg_read_huff_table(jpeg, data, i);
                break;
            case SOS:
                // read data as a scan
                jpeg_read_scan(jpeg, data, i);
                break;
            case APP0:
            case APP1:
            case APP2:
            case APP3:
            case APP4:
            case APP5:
            case APP6:
            case APP7:
            case APP8:
            case APP9:
            case APP10:
            case APP11:
            case APP12:
            case APP13:
            case APP14:
            case APP15:
                // read data as an application segment
                jpeg_read_app_seg(jpeg, data, i);
                break;
            case EOI:
            case SOI:
            case JPG:
            case DAC:
            case DNL:
            case DRI:
            case RST0:
            case RST1:
            case RST2:
            case RST3:
            case RST4:
            case RST5:
            case RST6:
            case RST7:
            case COM:
                // ignore these markers
                break;
            default:
                // unknown marker
                printf("Unknown marker: %x", marker);
                return false;
        }
    }

    // free the data
    free(data);

    return true;
}

/// @brief The jpeg_write_app_seg function writes an application segment to the
///        given file.
/// @param app_seg The application segment to write.
/// @param file The file to write to.
/// @param count The number of the application segment.
/// @return True if the segment was written successfully, false otherwise.
bool jpeg_write_app_seg(APP_SEG* app_seg, FILE* file, int count) {
    // write the start marker
    fprintf(file, "%c", START);

    // write the marker based on the count
    switch(count) {
        case 0:
            fprintf(file, "%c", APP0);
            break;
        case 1:
            fprintf(file, "%c", APP1);
            break;
        case 2:
            fprintf(file, "%c", APP2);
            break;
        case 3:
            fprintf(file, "%c", APP3);
            break;
        case 4:
            fprintf(file, "%c", APP4);
            break;
        case 5:
            fprintf(file, "%c", APP5);
            break;
        case 6:
            fprintf(file, "%c", APP6);
            break;
        case 7:
            fprintf(file, "%c", APP7);
            break;
        case 8:
            fprintf(file, "%c", APP8);
            break;
        case 9:
            fprintf(file, "%c", APP9);
            break;
        case 10:
            fprintf(file, "%c", APP10);
            break;
        case 11:
            fprintf(file, "%c", APP11);
            break;
        case 12:
            fprintf(file, "%c", APP12);
            break;
        case 13:
            fprintf(file, "%c", APP13);
            break;
        case 14:
            fprintf(file, "%c", APP14);
            break;
        case 15:
            fprintf(file, "%c", APP15);
            break;
        default:
            printf("Invalid APP segment count: %d", count);
            return false;
    }

    // write the data in the segment
    char zero = 0;
    for(int i = 0; i < app_seg->length; i++)
        fprintf(file, "%c", (app_seg->data[i] == '\0') ? zero : app_seg->data[i]);

    return true;
}

/// @brief The jpeg_write_huff_table function writes a huffman table to the
///        given file.
/// @param table The huffman table to write.
/// @param file The file to write to.
/// @return True if the table was written successfully, false otherwise.
bool jpeg_write_huff_table(HUFF_TABLE* table, FILE* file) {
    // write the start marker and the huffman table marker
    fprintf(file, "%c%c", START, DHT);

    // write the data in the table
    char zero = 0;
    for(int i = 0; i < table->length; i++)
        fprintf(file, "%c", (table->data[i] == '\0') ? zero : table->data[i]);
    
    return true;
}

/// @brief The jpeg_write_frame function writes a frame to the given file.
/// @param frame The frame to write.
/// @param file The file to write to.
/// @param count The number of the frame.
/// @return True if the frame was written successfully, false otherwise.
bool jpeg_write_frame(FRAME* frame, FILE* file, int count) {
    // write the start marker
    fprintf(file, "%c", START);

    // write the marker based on the count
    switch(count) {
        case 0:
            fprintf(file, "%c", SOF0);
            break;
        case 1:
            fprintf(file, "%c", SOF1);
            break;
        case 2:
            fprintf(file, "%c", SOF2);
            break;
        case 3:
            fprintf(file, "%c", SOF3);
            break;
        case 4:
            fprintf(file, "%c", SOF5);
            break;
        case 5:
            fprintf(file, "%c", SOF6);
            break;
        case 6:
            fprintf(file, "%c", SOF7);
            break;
        case 7:
            fprintf(file, "%c", SOF9);
            break;
        case 8:
            fprintf(file, "%c", SOF10);
            break;
        case 9:
            fprintf(file, "%c", SOF11);
            break;
        case 10:
            fprintf(file, "%c", SOF13);
            break;
        case 11:
            fprintf(file, "%c", SOF14);
            break;
        case 12:
            fprintf(file, "%c", SOF15);
            break;
        default:
            printf("Invalid frame count: %d", count);
            return false;
    }

    // write the data in the frame
    char zero = 0;
    for(int i = 0; i < frame->length; i++)
        fprintf(file, "%c", (frame->data[i] == '\0') ? zero : frame->data[i]);
    
    return true;
}

/// @brief The jpeg_write_quant_table function writes a quantization table to
///        the given file.
/// @param table The quantization table to write.
/// @param file The file to write to.
/// @return True if the table was written successfully, false otherwise.
bool jpeg_write_quant_table(QUANT_TABLE* table, FILE* file) {
    // write the start marker and the quantization table marker
    fprintf(file, "%c%c", START, DQT);

    // write the data in the table
    char zero = 0;
    for(int i = 0; i < table->length; i++)
        fprintf(file, "%c", (table->data[i] == '\0') ? zero : table->data[i]);
    
    return true;
}

/// @brief The jpeg_write_scan function writes a scan to the given file.
/// @param scan The scan to write.
/// @param file The file to write to.
/// @return True if the scan was written successfully, false otherwise.
bool jpeg_write_scan(SCAN* scan, FILE* file) {
    // write the start marker and the scan marker
    fprintf(file, "%c%c", START, SOS);

    // write the data in the scan
    char zero = 0;
    for(int i = 0; i < scan->length; i++)
        fprintf(file, "%c", (scan->data[i] == '\0') ? zero : scan->data[i]);
    
    return true;
}

/// @brief The jpeg_write function writes a jpeg to the given file.
/// @param jpeg The jpeg to write.
/// @param file The file to write to.
/// @return True if the jpeg was written successfully, false otherwise.
bool jpeg_write(JPEG* jpeg, FILE* file) {
    // write the start of image marker
    fprintf(file, "%c%c", START, SOI);

    // write the app segments
    int i;
    for(i = 0; i < jpeg->num_app_segments; i++)
        jpeg_write_app_seg(jpeg->app_segments + i, file, i);

    // write the quantization tables
    for(i = 0; i < jpeg->num_quant_tables; i++)
        jpeg_write_quant_table(jpeg->quant_tables + i, file);

    // write the frames
    for(i = 0; i < jpeg->num_frames; i++)
        jpeg_write_frame(jpeg->frames + i, file, i);

    // write the huffman tables
    for(i = 0; i < jpeg->num_huff_tables; i++)
        jpeg_write_huff_table(jpeg->huff_tables + i, file);

    // write the scans
    for(i = 0; i < jpeg->num_scans; i++)
        jpeg_write_scan(jpeg->scans + i, file);

    // write the end of image marker
    fprintf(file, "%c%c", START, EOI);

    return true;
}

/// @brief The jpeg_free function frees the memory allocated to the given jpeg.
/// @param jpeg The jpeg to free.
void jpeg_free(JPEG* jpeg) {
    // check if the jpeg is null
    if(jpeg == NULL)
        return;

    // free the frames
    for(int i = 0; i < jpeg->num_frames; i++)
        free(jpeg->frames[i].data);
    free(jpeg->frames);

    // free the quantization tables
    for(int i = 0; i < jpeg->num_quant_tables; i++)
        free(jpeg->quant_tables[i].data);
    free(jpeg->quant_tables);

    // free the huffman tables
    for(int i = 0; i < jpeg->num_huff_tables; i++)
        free(jpeg->huff_tables[i].data);
    free(jpeg->huff_tables);

    // free the scans
    for(int i = 0; i < jpeg->num_scans; i++)
        free(jpeg->scans[i].data);
    free(jpeg->scans);

    // free the app segments
    for(int i = 0; i < jpeg->num_app_segments; i++)
        free(jpeg->app_segments[i].data);
    free(jpeg->app_segments);

    // free the jpeg
    free(jpeg);
}
