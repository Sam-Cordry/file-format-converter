#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "png.h"

bool is_valid_ext(char* extension) {
    return strcmp(extension, ".png") == 0 || strcmp(extension, ".jpg") == 0 ||\
        strcmp(extension, ".jpeg") == 0;
}

int find_extension(char* filename) {
    for(size_t i = 0; i < strlen(filename); i++) {
        if(filename[i] == '.') {
            return i;
        }
    }
    return -1;
}

int main(int argc, char** argv) {
    if(argc > 5) {
        printf("Usage: fcc [-o/--overwrite] [-v/--verbose] ");
        printf("[-p/--pedantic] [filename]\n");
        return EXIT_FAILURE;
    }

    char* filename = "test.png";
    char* extension = &filename[find_extension(filename)];

    if(!is_valid_ext(extension)) {
        printf("Error: Provided file is not supported.\n");
        return EXIT_FAILURE;
    }

    bool overwrite = false;
    bool verbose = false;
    bool pedantic = false;
    for(int i = 1; i < argc - 1; i++) {
        if(strcmp(argv[i], "--overwrite") == 0 || strcmp(argv[i], "-o") == 0)
            overwrite = true;
        else if(strcmp(argv[i], "--verbose") == 0 ||\
                                strcmp(argv[i], "-v") == 0)
            verbose = true;
        else if(strcmp(argv[i], "--pedantic") == 0 ||\
                                strcmp(argv[i], "-p") == 0)
            pedantic = true;
        else {
            printf("Error: Invalid argument provided.\n");
            printf("Usage: fcc [-o/--overwrite] [-v/--verbose] [-p/--pedantic] [filename]\n");
            return EXIT_FAILURE;
        }
    }

    if(overwrite)
        printf("Overwrite mode enabled.\n");
    
    if(verbose)
        printf("Verbose mode enabled.\n");
    
    if(pedantic)
        printf("Pedantic mode enabled.\n");
    
    FILE* start_file = fopen(filename, "r");
    if(start_file == NULL) {
        printf("Error: Unable to open file.\n");
        return EXIT_FAILURE;
    }

    FILE* end_file = fopen("result.png", "r");
    if(end_file != NULL && !overwrite) {
        printf("Error: File already exists.\n");
        return EXIT_FAILURE;
    }

    if(strcmp(extension, ".png") == 0) {
        PNG* png = png_create();
        if(!png_read_direct(png, start_file)) {
            printf("Error: Unable to read PNG file.\n");
            return EXIT_FAILURE;
        }

        end_file = fopen("result.png", "w");
        if(!png_write_direct(png, end_file)) {
            printf("Error: Unable to write PNG file.\n");
            return EXIT_FAILURE;
        }
        png_free(png);
    }

    fclose(start_file);
    fflush(end_file);
    fclose(end_file);
    
    return EXIT_SUCCESS;
}
