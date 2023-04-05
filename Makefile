CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99
PATH=src
SRCS=$(wildcard $(PATH)/*.c)

ffc: $(PATH)/ffc.o $(PATH)/png.o $(PATH)/jpeg.o
	$(CC) $(CFLAGS) $(PATH)/ffc.o $(PATH)/png.o $(PATH)/jpeg.o -o ffc

$(PATH)/fcc.o: $(PATH)/ffc.c
	$(CC) $(CFLAGS) -c -o $(PATH)/ffc.o $(PATH)/ffc.c

$(PATH)/png.o: $(PATH)/png.c
	$(CC) $(CFLAGS) -c -o $(PATH)/png.o $(PATH)/png.c

$(PATH)/jpeg.o: $(PATH)/jpeg.c
	$(CC) $(CFLAGS) -c -o $(PATH)/jpeg.o $(PATH)/jpeg.c

# $(PATH)/tiff.o: $(PATH)/tiff.c
# 	$(CC) $(CFLAGS) -c -o $(PATH)/tiff.o $(PATH)/tiff.c

clean:
	/bin/rm -f $(PATH)/*.o
	/bin/rm -f result.*

realclean: clean
	/bin/rm -f ffc