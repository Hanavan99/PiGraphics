LIB_OBJS = graphics.o
TEST_OBJS = test.o
LDFLAGS += -lm -L.
CC = gcc

all: libgfx.so test

test: $(TEST_OBJS) libgfx.so
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) -lgfx $(LDFLAGS)

libgfx.so: $(LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ -shared $< $(LDFLAGS)

graphics.o: graphics.h

test.o: graphics.h
