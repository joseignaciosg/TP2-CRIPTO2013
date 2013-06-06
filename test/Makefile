CC=gcc
LD=gcc
CFLAGS=-g -W -Wall -pedantic -Wextra -lssl -lm -largtable2 -lcrypto
LDFLAGS=-g -lssl -lm -largtable2 -lcrypto

all: testbmp test testutil

testbmp: testbmp.o ../src/bmp.o ../src/stegobmp_write.o ../src/stegobmp_read.o ../src/util.o ../src/crypt.o

testcrypt: testcrypt.o ../src/crypt.o

test: test.o ../src/stegobmp_write.o ../src/stegobmp_read.o ../src/bmp.o ../src/util.o ../src/crypt.o
	gcc -g test.c ../src/stegobmp_write.c ../src/stegobmp_read.c ../src/bmp.c ../src/util.c ../src/crypt.c  -lm -lssl -largtable2 -lcrypto -o test

testutil: testutil.c ../src/util.o

.PHONY: clean mr-proper

clean:
	rm -f *.o


mr-proper: clean
	rm -f $(TEST_EXEC)
