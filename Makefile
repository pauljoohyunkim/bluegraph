CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-lbluetooth -lncurses
BIN=bluegraph

bin: obj/bluegraph.o obj/conn.o obj/transaction.o obj/capsule.o obj/tui.o
	$(CC) $(CFLAGS) $^ -o bin/$(BIN) $(LDFLAGS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

test/discoverytest: test/discoverytest.c src/conn.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/servertest: test/servertest.c src/conn.c src/transaction.c src/capsule.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/clienttest: test/clienttest.c src/conn.c src/transaction.c src/capsule.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test/serialtest: test/serialtest.c src/capsule.c src/transaction.c src/capsule.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean:
	$(RM) -f obj/*
	$(RM) -f bin/*
	$(RM) -f test/*test