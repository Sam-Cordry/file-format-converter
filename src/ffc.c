#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "png.h"
#include "jpeg.h"

bool is_valid_ext(char* extension) {
    return strcmp(extension, "png") == 0 || strcmp(extension, "jpg") == 0 ||
        strcmp(extension, "jpeg") == 0;
}

int find_extension(char* filename) {
    for(size_t i = 0; i < strlen(filename); i++) {
        if(filename[i] == '.') {
            return i + 1;
        }
    }
    return strlen(filename) - 1;
}

bool is_option(char* arg) {
    return strcmp(arg, "-o") == 0 || strcmp(arg, "--overwrite") == 0 ||
        strcmp(arg, "-v") == 0 || strcmp(arg, "--verbose") == 0;
}

int main(int argc, char** argv) {
    if(argc > 5) {
        printf("Usage: fcc [-o/--overwrite] [-v/--verbose] [filename]\n");
        return EXIT_FAILURE;
    }

    if(strcmp(argv[argc - 1], "--help") == 0 || strcmp(argv[argc - 1], "-h") == 0) {
        printf("Command: fcc\n");
        printf("Usage: fcc [-o/--overwrite] [-v/--verbose] ");
        printf("[filename]\n");
        printf("Options:\n");
        printf("\t-o, --overwrite\t\tAutomatically overwrite converted file (if one exists already).\n");
        printf("\t-v, --verbose\t\tPrint additional information.\n");
        return EXIT_SUCCESS;
    }

    bool overwrite = false;
    bool verbose = false;
    if(argc > 1) {
        for(int i = 1; i < argc - 1; i++) {
            if(strcmp(argv[i], "--overwrite") == 0 || strcmp(argv[i], "-o") == 0)
                overwrite = true;
            else if(strcmp(argv[i], "--verbose") == 0 ||
                                    strcmp(argv[i], "-v") == 0)
                verbose = true;
            else if(strcmp(argv[i], "-ov") == 0 || strcmp(argv[i], "-vo") == 0) {
                overwrite = true;
                verbose = true;
            }
            else {
                printf("Error: Invalid argument provided.\n");
                printf("Usage: fcc [-o/--overwrite] [-v/--verbose] [filename]\n");
                return EXIT_FAILURE;
            }
        }
    }
    
    if(verbose) {
        printf("Verbose mode enabled.\n");
        if(overwrite)
            printf("Overwrite mode enabled.\n");
    }

    char* filename = malloc(81 * sizeof(char));

    if(strcmp(argv[argc - 1], "./ffc") == 0 || is_option(argv[argc - 1])) {
        printf("Please enter a filename: ");
        scanf("%s", filename);
    } else {
        filename = argv[argc - 1];
    }

    char* extension = &filename[find_extension(filename)];

    if(*extension == '\0')
        scanf("An extension was not found.\nPlease enter the format of the file (png or bmp): %s", extension);

    if(!is_valid_ext(extension)) {
        printf("Error: Provided file format is not currently supported.\n");
        return EXIT_FAILURE;
    }
    
    FILE* start_file = fopen(filename, "r");
    if(start_file == NULL) {
        printf("Error: Unable to open file.\n");
        return EXIT_FAILURE;
    }

    char* end_extension = malloc(5);
    printf("What should the output file format be (png, jpeg, or jpg)? ");
    scanf("%s", end_extension);

    if(!is_valid_ext(end_extension)) {
        printf("Error: Provided file format is not currently supported.\n");
        return EXIT_FAILURE;
    }

    char* end_filename = malloc(81);
    end_filename[0] = '\0';
    printf("What should the output file be named? ");
    scanf("%s", end_filename);
    // if(strcmp(end_filename, "") == 0) {
    //     end_filename = "result.";
    //     strcat(end_filename, extension);
    // }

    FILE* end_file = fopen(end_filename, "r");
    if(end_file != NULL && !overwrite) {
        printf("Error: File already exists. Run again with -o or --overwrite to overwrite this file.\n");
        return EXIT_FAILURE;
    }
    fclose(end_file);

    if(strcmp(extension, ".png") == 0) {
        PNG* png = png_create();
        if(!png_read(png, start_file)) {
            printf("Error: Unable to read PNG file.\n");
            return EXIT_FAILURE;
        }

        end_file = fopen(end_filename, "w");
        if(!png_write(png, end_file)) {
            printf("Error: Unable to write PNG file.\n");
            return EXIT_FAILURE;
        }
        png_free(png);
    } else if(strcmp(extension, ".jpeg") == 0 || strcmp(extension, ".jpg")) {
        JPEG* jpeg = jpeg_create();
        if(!jpeg_read(jpeg, start_file)) {
            printf("Error: Unable to read JPEG file.\n");
            return EXIT_FAILURE;
        }

        end_file = fopen(end_filename, "w");
        if(!jpeg_write(jpeg, end_file)) {
            printf("Error: Unable to write JPEG file.\n");
            return EXIT_FAILURE;
        }
    }

    fclose(start_file);
    fclose(end_file);
    
    return EXIT_SUCCESS;
}
