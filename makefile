CC = gcc
CFLAGS = -Wall -Wextra --std=gnu99

SRC = bitmap.c buddy_allocator.c pseudo_malloc.c test_pseudomalloc.c
OBJS = $(SRC:.c=.o)
TARGET = test_pseudomalloc	# non sicuro

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean