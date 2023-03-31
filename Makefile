CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99
PATH=src
SRCS=$(wildcard $(PATH)/*.c)

ffc: $(PATH)/ffc.o $(PATH)/png.o
	$(CC) $(CFLAGS) $(PATH)/ffc.o $(PATH)/png.o -o ffc

$(PATH)/fcc.o: $(PATH)/ffc.c
	$(CC) $(CFLAGS) -c -o $(PATH)/ffc.o $(PATH)/ffc.c

$(PATH)/png.o: $(PATH)/png.c
	$(CC) $(CFLAGS) -c -o $(PATH)/png.o $(PATH)/png.c

clean:
	/bin/rm -f $(PATH)/*.o

realclean: clean
	/bin/rm -f ffc