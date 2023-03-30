CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99
PATH=src
SRCS=$(wildcard $(PATH)/*.c)

fcc: $(PATH)/fcc.o
	$(CC) -o ffc $(CFLAGS) $(PATH)/ffc.o

$(PATH)/fcc.o: $(PATH)/png.o
	$(CC) $(CFLAGS) -c -o $(PATH)/ffc.o $(PATH)/ffc.c

clean:
	/bin/rm -f $(PATH)/*.o

realclean: clean
	/bin/rm -f ffc