CFLAGS= -W -Wall -pedantic
LFLAGS=-lssl

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
EXEC=stegobmp

$(EXEC): $(OBJS)

.PHONY: clean mr-proper

clean:
	rm -rf *.o

mr-proper: clean
	rm $(EXEC)
