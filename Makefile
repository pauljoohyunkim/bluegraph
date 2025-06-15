CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-lbluetooth

test/discoverytest: test/discoverytest.c src/conn.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/servertest: test/servertest.c src/conn.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/clienttest: test/clienttest.c src/conn.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/serialtest: test/serialtest.c src/capsule.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean:
	rm -f test/*test