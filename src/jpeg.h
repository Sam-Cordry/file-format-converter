#ifndef JPEG_H
#define JPEG_H

#define SOI 0xD8
#define EOI 0xD9


JPEG jpeg_create();
bool jpeg_read(JPEG jpeg, FILE * file);
bool jpeg_write(JPEG jpeg, FILE * file);
void jpeg_free(JPEG jpeg);

#endif
