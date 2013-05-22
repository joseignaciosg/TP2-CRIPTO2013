CFLAGS= -W -Wall -pedantic

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
EXEC=stegobmp

$(EXEC): $(OBJS)

.PHONY: clean mr-proper

clean:
	rm -rf *.o

mr-proper: clean
	rm $(EXEC)
