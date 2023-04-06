# Author: Sam Cordry
# Makefiile for Fike Format Converter (FFC)

# set compiler, flags, and path to source files
CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99
PATH=src
SRCS=$(wildcard $(PATH)/*.c)

# make all
ffc: $(PATH)/ffc.o $(PATH)/png.o $(PATH)/jpeg.o
	$(CC) $(CFLAGS) $(PATH)/ffc.o $(PATH)/png.o $(PATH)/jpeg.o -o ffc

# make object files
$(PATH)/fcc.o: $(PATH)/ffc.c
	$(CC) $(CFLAGS) -c -o $(PATH)/ffc.o $(PATH)/ffc.c

$(PATH)/png.o: $(PATH)/png.c
	$(CC) $(CFLAGS) -c -o $(PATH)/png.o $(PATH)/png.c

$(PATH)/jpeg.o: $(PATH)/jpeg.c
	$(CC) $(CFLAGS) -c -o $(PATH)/jpeg.o $(PATH)/jpeg.c

# make clean, removes object files and results
clean:
	/bin/rm -f $(PATH)/*.o
	/bin/rm -f result*.*

# make realclean, removes executable
realclean: clean
	/bin/rm -f ffc
