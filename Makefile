CC=gcc
CFLAGS="-Wall"

debug:clean
	$(CC) $(CFLAGS) -g -o filesystemTester test_main.h test_main.c test_util.c test_func.c test_test.c
stable:clean
	$(CC) $(CFLAGS) -o filesystemTester test_main.h test_main.c test_util.c test_func.c test_test.c
clean:
	rm -vfr *~ filesystemTester
