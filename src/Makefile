CFLAGS= -W -Wall -pedantic -Wextra --std=c99
LDFLAGS=-lssl -lcrypto -lm

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
EXEC=stegobmp

$(EXEC): $(OBJS)

.PHONY: clean mr-proper

stegobmp: 
	gcc stegobmp.c stegobmp_write.c stegobmp_read.c bmp.c util.c crypt.c  -lm -lssl -lcrypto -o stegobmp


clean:
	rm -rf *.o

mr-proper: clean
	rm $(EXEC)
