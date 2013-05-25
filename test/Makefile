CFLAGS=-W -Wall -pedantic

TEST_EXEC=testbmp test

testbmp: testbmp.o ../src/bmp.o


.PHONY: clean mr-proper

clean:
	rm -f *.o


mr-proper: clean
	rm -f $(TEST_EXEC)
