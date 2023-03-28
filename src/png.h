#ifndef PNG_H
#define PNG_H

const char PNG_HEADER[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
const char IHDR_HEADER[4] = {0x49, 0x48, 0x44, 0x52};
const char IDAT_HEADER[4] = {0x49, 0x44, 0x41, 0x54};
const char IEND_HEADER[4] = {0x49, 0x45, 0x4D, 0x44};

#ifndef PNG_IMPL
typedef struct { } * PNG;
#endif

#ifndef IHDR_IMPL
typedef struct { } * IHDR;
#endif

#ifndef PLTE_IMPL
typedef struct { } * PLTE;
#endif

#ifndef IDAT_IMPL
typedef struct { } * IDAT;
#endif

#ifndef IEND_IMPL
typedef struct { } * IEND;
#endif

bool is_png_header(const char * header);
bool is_ihdr_header(const char * header);
bool is_idat_header(const char * header);
bool is_iend_header(const char * header);

bool read_ihdr(PNG * png, FILE * file);
bool read_idat(PNG * png, FILE * file);
bool read_plte(PNG * png, FILE * file);
bool read_iend(PNG * png, FILE * file);

bool init_png(PNG * png);
bool read_png_direct(PNG * png, FILE * file);
bool read_png_bitmap(PNG * png, FILE * file);
bool write_png_direct(PNG * png, FILE * file);
bool write_png_bitmap(PNG * png, FILE * file);
bool destroy_png(PNG * png);

#endif
