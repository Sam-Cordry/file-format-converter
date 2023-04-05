#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "jpeg.h"

#define MEM_CHECK(ptr) if(ptr == NULL) { printf("Unable to allocate memory");\
                                            return false; }
#define FEOF_CHECK(file) if(feof(file)) { printf("Unexpected end of file");\
                                            return false; }

void print_info(unsigned char* data, int length) {
    printf("Ran with length: %d\nData:\n", length);
    for(int i = 0; i < length; i++) {
        printf("%02X", data[i]);
        if(i % 2 == 1)
            printf(" ");
    }
    printf("\n\n");
}

JPEG* jpeg_create(void) {
    JPEG* jpeg = malloc(sizeof(JPEG));
    
    jpeg->frames = NULL;
    jpeg->quantization_tables = NULL;
    jpeg->huffman_tables = NULL;
    jpeg->scans = NULL;
    jpeg->app_segments = NULL;

    jpeg->frames_length = 0;
    jpeg->quantization_tables_length = 0;
    jpeg->huffman_tables_length = 0;
    jpeg->scans_length = 0;
    jpeg->app_segments_length = 0;

    return jpeg;
}

bool jpeg_read_frame(JPEG* jpeg, unsigned char* data, int length) {
    if(jpeg->frames_length == 0) {
        jpeg->frames_length = 1;
        jpeg->frames = malloc(sizeof(FRAME));
    } else {
        jpeg->frames_length++;
        jpeg->frames = realloc(jpeg->frames, sizeof(FRAME) * jpeg->frames_length);
    }
    MEM_CHECK(jpeg->frames);

    FRAME* frame = &jpeg->frames[jpeg->frames_length - 1];

    frame->data = malloc(length);

    frame->length = length;
    MEM_CHECK(frame->data);

    memcpy(frame->data, data, length);

    return true;
}

bool jpeg_read_quant_table(JPEG* jpeg, unsigned char* data, int length) {
    if(jpeg->quantization_tables_length == 0) {
        jpeg->quantization_tables_length = 1;
        jpeg->quantization_tables = malloc(sizeof(QUANT_TABLE));
    } else {
        jpeg->quantization_tables_length++;
        jpeg->quantization_tables = realloc(jpeg->quantization_tables, sizeof(QUANT_TABLE) * jpeg->quantization_tables_length);
    }
    MEM_CHECK(jpeg->quantization_tables);

    QUANT_TABLE* table = &jpeg->quantization_tables[jpeg->quantization_tables_length - 1];

    table->data = malloc(length);

    table->length = length;
    MEM_CHECK(table->data);

    memcpy(table->data, data, length);

    return true;
}

bool jpeg_read_huff_table(JPEG* jpeg, unsigned char* data, int length) {
    if(jpeg->huffman_tables_length == 0) {
        jpeg->huffman_tables_length = 1;
        jpeg->huffman_tables = malloc(sizeof(HUFF_TABLE));
    } else {
        jpeg->huffman_tables_length++;
        jpeg->huffman_tables = realloc(jpeg->huffman_tables, sizeof(HUFF_TABLE) * jpeg->huffman_tables_length);
    }

    HUFF_TABLE* table = &jpeg->huffman_tables[jpeg->huffman_tables_length - 1];

    table->data = malloc(length);

    table->length = length;
    MEM_CHECK(table->data);

    memcpy(table->data, data, length);

    return true;
}

bool jpeg_read_scan(JPEG* jpeg, unsigned char* data, int length) {
    if(jpeg->scans_length == 0) {
        jpeg->scans_length = 1;
        jpeg->scans = malloc(sizeof(SCAN));
    } else {
        jpeg->scans_length++;
        jpeg->scans = realloc(jpeg->scans, sizeof(SCAN) * jpeg->scans_length);
    }

    SCAN* scan = &jpeg->scans[jpeg->scans_length - 1];

    scan->data = malloc(length);

    scan->length = length;
    MEM_CHECK(scan->data);

    memcpy(scan->data, data, length);

    return true;
}

bool jpeg_read_app_seg(JPEG* jpeg, unsigned char* data, int length) {
    if(jpeg->app_segments_length == 0) {
        jpeg->app_segments_length = 1;
        jpeg->app_segments = malloc(sizeof(APP_SEG));
    } else {
        jpeg->app_segments_length++;
        jpeg->app_segments = realloc(jpeg->app_segments, sizeof(APP_SEG) * jpeg->app_segments_length);
    }

    APP_SEG* app_seg = &jpeg->app_segments[jpeg->app_segments_length - 1];

    app_seg->data = malloc(length);

    app_seg->length = length;
    MEM_CHECK(app_seg->data);

    memcpy(app_seg->data, data, length);

    return true;
}

bool jpeg_read(JPEG* jpeg, FILE* file) {
    if(fgetc(file) != START) {
        printf("Invalid JPEG file\n");
        return false;
    }
    unsigned char marker;
    unsigned char* data = malloc(MAX_DATA_LENGTH + 1);
    while(!feof(file)) {
        marker = fgetc(file);
        FEOF_CHECK(file);
        
        char c;
        int i;
        for(i = 0; (unsigned char) (c = fgetc(file)) != START; i++) {
            data[i] = (unsigned char) c;
            FEOF_CHECK(file);
        }
        data[i] = '\0';

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
                jpeg_read_frame(jpeg, data, i);
                break;
            case DQT:
                jpeg_read_quant_table(jpeg, data, i);
                break;
            case DHT:
                jpeg_read_huff_table(jpeg, data, i);
                break;
            case SOS:
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
                break;
            default:
                printf("Unknown marker: %x", marker);
                return false;
        }
    }
    free(data);
    return true;
}

bool jpeg_write_app_seg(APP_SEG* app_seg, FILE* file, int count) {
    fprintf(file, "%c", START);

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

    char zero = 0;
    for(int i = 0; i < app_seg->length; i++)
        fprintf(file, "%c", (app_seg->data[i] == '\0') ? zero : app_seg->data[i]);

    return true;
}

bool jpeg_write_huff_table(HUFF_TABLE* table, FILE* file) {
    fprintf(file, "%c%c", START, DHT);

    char zero = 0;
    for(int i = 0; i < table->length; i++)
        fprintf(file, "%c", (table->data[i] == '\0') ? zero : table->data[i]);
    
    return true;
}

bool jpeg_write_frame(FRAME* frame, FILE* file, int count) {
    fprintf(file, "%c", START);

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

    char zero = 0;
    for(int i = 0; i < frame->length; i++)
        fprintf(file, "%c", (frame->data[i] == '\0') ? zero : frame->data[i]);
    
    return true;
}

bool jpeg_write_quant_table(QUANT_TABLE* table, FILE* file) {
    fprintf(file, "%c%c", START, DQT);

    char zero = 0;
    for(int i = 0; i < table->length; i++)
        fprintf(file, "%c", (table->data[i] == '\0') ? zero : table->data[i]);
    
    return true;
}

bool jpeg_write_scan(SCAN* scan, FILE* file) {
    fprintf(file, "%c%c", START, SOS);

    char zero = 0;
    for(int i = 0; i < scan->length; i++)
        fprintf(file, "%c", (scan->data[i] == '\0') ? zero : scan->data[i]);
    
    return true;
}

bool jpeg_write(JPEG* jpeg, FILE* file) {
    fprintf(file, "%c%c", START, SOI);

    int i;
    for(i = 0; i < jpeg->app_segments_length; i++)
        jpeg_write_app_seg(&jpeg->app_segments[i], file, i);

    for(i = 0; i < jpeg->quantization_tables_length; i++)
        jpeg_write_quant_table(&jpeg->quantization_tables[i], file);

    for(i = 0; i < jpeg->frames_length; i++)
        jpeg_write_frame(&jpeg->frames[i], file, i);

    for(i = 0; i < jpeg->huffman_tables_length; i++)
        jpeg_write_huff_table(&jpeg->huffman_tables[i], file);

    for(i = 0; i < jpeg->scans_length; i++)
        jpeg_write_scan(&jpeg->scans[i], file);

    fprintf(file, "%c%c", START, EOI);

    return true;
}

void jpeg_free(JPEG* jpeg) {
    if(jpeg == NULL)
        return;

    for(int i = 0; i < jpeg->frames_length; i++)
        free(jpeg->frames[i].data);
    free(jpeg->frames);

    for(int i = 0; i < jpeg->quantization_tables_length; i++)
        free(jpeg->quantization_tables[i].data);
    free(jpeg->quantization_tables);

    for(int i = 0; i < jpeg->huffman_tables_length; i++)
        free(jpeg->huffman_tables[i].data);
    free(jpeg->huffman_tables);

    for(int i = 0; i < jpeg->scans_length; i++)
        free(jpeg->scans[i].data);
    free(jpeg->scans);

    free(jpeg);
}
