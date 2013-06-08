CFLAGS= -W -Wall -pedantic
LDFLAGS=-lssl -lcrypto -lm

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
EXEC=stegobmp

$(EXEC): $(OBJS)

.PHONY: clean mr-proper

clean:
	rm -rf *.o

mr-proper: clean
	rm $(EXEC)
