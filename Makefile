CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-lbluetooth

test/discoverytest: test/discoverytest.c src/conn.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/servertest: test/servertest.c src/conn.c src/transaction.c src/capsule.c src/storage.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/clienttest: test/clienttest.c src/conn.c src/transaction.c src/capsule.c src/storage.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/serialtest: test/serialtest.c src/capsule.c src/transaction.c src/capsule.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/storagetest: test/storagetest.c src/storage.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/addresstranslationtest: test/addresstranslationtest.c src/storage.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/test: test/test.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
clean:
	rm -f test/*test