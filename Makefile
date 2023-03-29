CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99
PATH=src
SRCS=$(wildcard $(PATH)/*.c)

$(PATH)/fcc.o: $(SRCS)
	$(CC) $(PATH)/ffc.c -c -o $(PATH)/ffc.o $(CFLAGS)

fcc: $(PATH)/fcc.o
	$(CC) $(PATH)/ffc.o -o ffc $(CFLAGS)

clean:
	rm -f ffc
	rm -f $(PATH)/*.o