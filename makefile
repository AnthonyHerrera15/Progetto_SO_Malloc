CC = gcc
CFLAGS = -Wall -Wextra --std=gnu99

SRC = bitmap.c buddy_allocator.c pseudo_malloc.c test_pseudomalloc.c
OBJS = $(SRC:.c=.o)
TARGET = test_pseudomalloc

# Default target : costruisce il file eseguibile
all: $(TARGET)

# Regola : utilizza i file oggetto generati dalla compilazione dei file sorgente
# 		   e li collega per creare l'eseguibile finale
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Regola : compila i file sorgente in file oggetto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regola : pulisce i file oggetto e l'eseguibile
clean:
	rm -f $(OBJS) $(TARGET)