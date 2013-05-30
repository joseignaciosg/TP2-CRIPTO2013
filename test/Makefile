CFLAGS=-W -Wall -pedantic -Wextra

all: testbmp test testutil

testbmp: testbmp.o ../src/bmp.o ../src/stegobmp_write.o ../src/util.o

test: test.o ../src/stegobmp_write.o ../src/bmp.o ../src/util.o

testutil: testutil.c ../src/util.o

.PHONY: clean mr-proper

clean:
	rm -f *.o


mr-proper: clean
	rm -f $(TEST_EXEC)
