#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool is_valid_file_ext(char * filename) {
    if(strcmp(&filename[strlen(filename) - 4], ".png") ||
        strcmp(&filename[strlen(filename) - 4], ".jpg") ||
        strcmp(&filename[strlen(filename) - 5], ".jpeg")) {
        return true;
    }
    return false;
}

int main(int argc, char ** argv) {
    if(argc > 4) {
        printf("Usage: fcc [--verbose] [--pedantic] [filename]");
        return EXIT_FAILURE;
    }

    if(!is_valid_file_ext(argv[argc - 1])) {
        printf("Error: Provided file is not currently supported.");
        return EXIT_FAILURE;
    }

    bool verbose = false;
    bool pedantic = false;
    for(int i = 1; i < argc - 1; i++) {
        if(strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        } else if(strcmp(argv[i], "--pedantic") == 0) {
            pedantic = true;
        } else {
            printf("Error: Invalid argument provided.");
            return EXIT_FAILURE;
        }
    }

    if(verbose)
        printf("Verbose mode enabled.\n");
    
    if(pedantic)
        printf("Pedantic mode enabled.\n");
    
    return EXIT_SUCCESS;
}
