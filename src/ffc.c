///
/// @file fcc.c
/// @brief The main file for the File Format Converter (FFC) program.
/// @author Sam Cordry

// include needed system headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// include the headers for the supported file formats
#include "png.h"
#include "jpeg.h"

/// @brief The is_valid_ext function checks if the given extension can be used.
/// @param extension The extension to check.
/// @return True if the extension is supported, false otherwise.
bool is_valid_ext(char* extension) {
    return strcmp(extension, "png") == 0 || strcmp(extension, "jpg") == 0 ||
        strcmp(extension, "jpeg") == 0;
}

/// @brief The find_extension function finds the index of the extension.
/// @param filename The filename to search for an extension in.
/// @return The index of the extension.
int find_extension(char* filename) {
    for(size_t i = strlen(filename) - 1; i >= 0; i--)
        if(filename[i] == '.')
            return i + 1;
    
    return 0;
}

/// @brief The is_option function checks if the given argument is an option.
/// @param arg The argument to check.
/// @return True if the argument is an option, false otherwise.
bool is_option(char* arg) {
    return strcmp(arg, "-o") == 0 || strcmp(arg, "--overwrite") == 0 ||
        strcmp(arg, "-v") == 0 || strcmp(arg, "--verbose") == 0;
}

/// @brief The main function for the File Format Converter (FFC) program.
/// @param argc The number of arguments.
/// @param argv The arguments.
/// @return The exit status of the program.
int main(int argc, char** argv) {
    // print usage statement if too many arguments are provided
    if(argc > 5) {
        printf("Usage: fcc [-o/--overwrite] [-v/--verbose] [filename]\n");
        return EXIT_FAILURE;
    }

    // print help statement if requested
    if(strcmp(argv[argc - 1], "--help") == 0 || strcmp(argv[argc - 1], "-h") == 0) {
        printf("Command: fcc\n");
        printf("Usage: fcc [-o/--overwrite] [-v/--verbose] [filename]\n");
        printf("Options:\n");
        printf("\t-o, --overwrite\t\tAutomatically overwrite converted file (if one exists already).\n");
        printf("\t-v, --verbose\t\tPrint additional information.\n");
        return EXIT_SUCCESS;
    }

    // search the arguments for requested options
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
    
    // print additional information if requested
    if(verbose) {
        printf("Verbose mode enabled.\n");
        if(overwrite)
            printf("Overwrite mode enabled.\n");
    }

    // create filename with default terminal width
    char filename[81];

    // if no filename is provided, prompt the user for one
    if(strcmp(argv[argc - 1], "./ffc") == 0 || is_option(argv[argc - 1])) {
        printf("Please enter a filename: ");
        scanf("%s", filename);
    } else
        strcpy(filename, argv[argc - 1]);

    int extension_index = find_extension(filename);
    char* extension = malloc(5);

    // requests file format if no extension is found
    if(extension_index == -1)
        scanf("An extension was not found.\nPlease enter the format of the file (png or bmp): %s", extension);
    else
        strcpy(extension, filename + extension_index);

    // check if the extension is valid
    if(!is_valid_ext(extension)) {
        printf("Error: Provided file format is not currently supported.\n");
        return EXIT_FAILURE;
    }
    
    // open the file
    FILE* start_file = fopen(filename, "r");
    if(start_file == NULL) {
        printf("Error: Unable to open file.\n");
        return EXIT_FAILURE;
    }

    // prompt the user for the output file format
    char* end_extension = malloc(5);
    printf("What should the output file format be (png, jpeg, or jpg)? ");
    scanf("%s", end_extension);

    // check if the extension is valid
    if(!is_valid_ext(end_extension)) {
        printf("Error: Provided file format is not currently supported.\n");
        return EXIT_FAILURE;
    }

    // prompt the user for the output filename
    char* end_filename = malloc(81);
    end_filename[0] = '\0';
    printf("What should the output file be named? ");
    scanf("%s", end_filename);

    // check if the output file already exists
    FILE* end_file = fopen(end_filename, "r");

    // if the file exists and overwrite is not enabled, exit
    if(end_file != NULL && !overwrite) {
        printf("Error: File already exists. Run again with -o or --overwrite to overwrite this file.\n");
        return EXIT_FAILURE;
    }
    fclose(end_file);
    
    // create pointers to structs for all supported file formats
    PNG* png;
    JPEG* jpeg;

    // read the file as the appropriate format
    if(strcmp(extension, "png") == 0) {
        png = png_create();
        if(!png_read(png, start_file)) {
            printf("Error: Unable to read PNG file.\n");
            return EXIT_FAILURE;
        }
    } else if(strcmp(extension, "jpeg") == 0 || strcmp(extension, "jpg") == 0) {
        jpeg = jpeg_create();
        if(!jpeg_read(jpeg, start_file)) {
            printf("Error: Unable to read JPEG file.\n");
            return EXIT_FAILURE;
        }
    }

    // write the file as the appropriate format
    if(strcmp(end_extension, "png") == 0) {
        end_file = fopen(end_filename, "w");
        if(!png_write(png, end_file)) {
            printf("Error: Unable to write PNG file.\n");
            return EXIT_FAILURE;
        }
        png_free(png);
    } else if(strcmp(end_extension, "jpeg") == 0 || strcmp(end_extension, "jpg") == 0) {
        end_file = fopen(end_filename, "w");
        if(!jpeg_write(jpeg, end_file)) {
            printf("Error: Unable to write JPEG file.\n");
            return EXIT_FAILURE;
        }
        jpeg_free(jpeg);
    }

    // close the files
    fclose(start_file);
    fclose(end_file);
    
    return EXIT_SUCCESS;
}
