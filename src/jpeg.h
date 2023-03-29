#ifndef JPEG_H
#define JPEG_H

#ifndef JPEG_IMPL
typedef struct { } * JPEG;
#endif

JPEG jpeg_init();
bool jpeg_read_direct(JPEG jpeg, FILE * file);
bool jpeg_read_bitmap(JPEG jpeg, FILE * file);
bool jpeg_write_direct(JPEG jpeg, FILE * file);
bool jpeg_write_bitmap(JPEG jpeg, FILE * file);
void jpeg_free(JPEG jpeg);

#endif
