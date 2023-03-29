#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool is_valid_ext(char * extension) {
    return !(strcmp(extension, ".png") && strcmp(extension, ".jpg") &&\
        strcmp(extension, ".jpeg"));
}

int find_extension(char * filename) {
    for(int i = 0; i < strlen(filename); i++) {
        if(filename[i] == '.') {
            return i;
        }
    }
    return -1;
}

int main(int argc, char ** argv) {
    if(argc > 5) {
        printf("Usage: fcc [-o/--overwrite] [-v/--verbose] ");
        printf("[-p/--pedantic] [filename]\n");
        return EXIT_FAILURE;
    }

    char * filename = "test.png";
    char * extension = &filename[find_extension(filename)];

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
            printf("Usage: fcc [-f/--force] [-v/--verbose] [-p/--pedantic] [filename]\n");
            return EXIT_FAILURE;
        }
    }

    if(overwrite)
        printf("Overwrite mode enabled.\n");
    
    if(verbose)
        printf("Verbose mode enabled.\n");
    
    if(pedantic)
        printf("Pedantic mode enabled.\n");
    
    FILE * start_file = fopen(filename, "r");
    if(start_file == NULL) {
        printf("Error: Unable to open file.\n");
        return EXIT_FAILURE;
    }

    if(verbose)
        printf("Read extension: %s\n", extension);

    fclose(start_file);
    
    return EXIT_SUCCESS;
}
