CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-lbluetooth

test/discoverytest: test/discoverytest.c src/conn.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean:
	rm -f test/*test