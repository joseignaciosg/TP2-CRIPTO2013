CFLAGS= -W -Wall -pedantic
LFLAGS=-lssl

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
EXEC=stegobmp

$(EXEC): $(OBJS)

.PHONY: clean mr-proper

stegobmp: 
	gcc -g stegobmp.c stegobmp_write.c stegobmp_read.c bmp.c util.c crypt.c  -lm -lssl -largtable2 -lcrypto -o stegobmp

clean:
	rm -rf *.o

mr-proper: clean
	rm $(EXEC)
