#ifndef JPEG_H
#define JPEG_H

#define START (unsigned char) 0xFF
#define SOF0 (unsigned char) 0xC0
#define SOF1 (unsigned char) 0xC1
#define SOF2 (unsigned char) 0xC2
#define SOF3 (unsigned char) 0xC3
#define DHT (unsigned char) 0xC4
#define SOF5 (unsigned char) 0xC5
#define SOF6 (unsigned char) 0xC6
#define SOF7 (unsigned char) 0xC7
#define JPG (unsigned char) 0xC8
#define SOF9 (unsigned char) 0xC9
#define SOF10 (unsigned char) 0xCA
#define SOF11 (unsigned char) 0xCB
#define DAC (unsigned char) 0xCC
#define SOF13 (unsigned char) 0xCD
#define SOF14 (unsigned char) 0xCE
#define SOF15 (unsigned char) 0xCF
#define RST0 (unsigned char) 0xD0
#define RST1 (unsigned char) 0xD1
#define RST2 (unsigned char) 0xD2
#define RST3 (unsigned char) 0xD3
#define RST4 (unsigned char) 0xD4
#define RST5 (unsigned char) 0xD5
#define RST6 (unsigned char) 0xD6
#define RST7 (unsigned char) 0xD7
#define SOI (unsigned char) 0xD8
#define EOI (unsigned char) 0xD9
#define SOS (unsigned char) 0xDA
#define DQT (unsigned char) 0xDB
#define DNL (unsigned char) 0xDC
#define DRI (unsigned char) 0xDD
#define DHP (unsigned char) 0xDE
#define EXP (unsigned char) 0xDF
#define APP0 (unsigned char) 0xE0
#define APP1 (unsigned char) 0xE1
#define APP2 (unsigned char) 0xE2
#define APP3 (unsigned char) 0xE3
#define APP4 (unsigned char) 0xE4
#define APP5 (unsigned char) 0xE5
#define APP6 (unsigned char) 0xE6
#define APP7 (unsigned char) 0xE7
#define APP8 (unsigned char) 0xE8
#define APP9 (unsigned char) 0xE9
#define APP10 (unsigned char) 0xEA
#define APP11 (unsigned char) 0xEB
#define APP12 (unsigned char) 0xEC
#define APP13 (unsigned char) 0xED
#define APP14 (unsigned char) 0xEE
#define APP15 (unsigned char) 0xEF
#define JPG0 (unsigned char) 0xF0
#define JPG1 (unsigned char) 0xF1
#define JPG2 (unsigned char) 0xF2
#define JPG3 (unsigned char) 0xF3
#define JPG4 (unsigned char) 0xF4
#define JPG5 (unsigned char) 0xF5
#define JPG6 (unsigned char) 0xF6
#define JPG7 (unsigned char) 0xF7
#define JPG8 (unsigned char) 0xF8
#define JPG9 (unsigned char) 0xF9
#define JPG10 (unsigned char) 0xFA
#define JPG11 (unsigned char) 0xFB
#define JPG12 (unsigned char) 0xFC
#define JPG13 (unsigned char) 0xFD
#define COM (unsigned char) 0xFE

#define MAX_DATA_LENGTH 65535

typedef struct {
    unsigned char* data;
    int length;
} FRAME;

typedef struct {
    unsigned char* data;
    int length;
} QUANT_TABLE;

typedef struct {
    unsigned char* data;
    int length;
} HUFF_TABLE;

typedef struct {
    unsigned char* data;
    int length;
} SCAN;

typedef struct {
    unsigned char* data;
    int length;
} APP_SEG;

typedef struct {
    FRAME* frames;
    QUANT_TABLE* quantization_tables;
    HUFF_TABLE* huffman_tables;
    SCAN* scans;
    APP_SEG* app_segments;
    int frames_length;
    int quantization_tables_length;
    int huffman_tables_length;
    int scans_length;
    int app_segments_length;
} JPEG;

JPEG* jpeg_create(void);

bool jpeg_read_frame(JPEG* jpeg, unsigned char* data, int length);
bool jpeg_read_quant_table(JPEG* jpeg, unsigned char* data, int length);
bool jpeg_read_huff_table(JPEG* jpeg, unsigned char* data, int length);
bool jpeg_read_scan(JPEG* jpeg, unsigned char* data, int length);
bool jpeg_read_app_seg(JPEG* jpeg, unsigned char* data, int length);
bool jpeg_read(JPEG* jpeg, FILE* file);

bool jpeg_write_app_seg(APP_SEG* jpeg, FILE* file, int count);
bool jpeg_write_huff_table(HUFF_TABLE* jpeg, FILE* file);
bool jpeg_write_frame(FRAME* jpeg, FILE* file, int count);
bool jpeg_write_quant_table(QUANT_TABLE* jpeg, FILE* file);
bool jpeg_write_scan(SCAN* jpeg, FILE* file);
bool jpeg_write(JPEG* jpeg, FILE* file);

void jpeg_free(JPEG* jpeg);

#endif
