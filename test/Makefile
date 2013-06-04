CC=gcc
LD=gcc
CFLAGS=-g -W -Wall -pedantic -Wextra
LDFLAGS=-g -lssl

all: testbmp test testutil

testbmp: testbmp.o ../src/bmp.o ../src/stegobmp_write.o ../src/util.o ../src/crypt.o

testcrypt: testcrypt.o ../src/crypt.o

test: test.o ../src/stegobmp_write.o ../src/bmp.o ../src/util.o ../src/crypt.o

testutil: testutil.c ../src/util.o

.PHONY: clean mr-proper

clean:
	rm -f *.o


mr-proper: clean
	rm -f $(TEST_EXEC)
