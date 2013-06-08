CFLAGS= -W -Wall -pedantic -Wextra --std=c99
LDFLAGS=-lcrypto -lssl

EXEC=stegobmp

all: $(EXEC) clean

.PHONY: clean mr-proper

$(EXEC): 
	cc $(CFLAGS) stegobmp.c stegobmp_write.c stegobmp_read.c bmp.c util.c crypt.c $(LDFLAGS) -o $(EXEC)

clean:
	rm -rf *.o

mr-proper: clean
	rm $(EXEC)
