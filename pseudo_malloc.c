#include "pseudo_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char memory[MEMORY_SIZE];
char bitmap_buffer[BITMAP_SIZE];
BuddyAllocator allocator;

// Inizializza il sistema di allocazione
void pseudo_init() {
    buddy_allocator_init(&allocator, memory, MEMORY_SIZE, MIN_BUCKET_SIZE, bitmap_buffer, BITMAP_SIZE);
}

// Alloca un blocco di memoria
void* pseudo_malloc(int size) {
    if (size <= 0) {
        printf("ERRORE: dimensione non valida\n");
        return NULL; 
    }
    if (size < THRESHOLD) {         // Se la dimensione è minore di THRESHOLD, usa il buddy allocator
        return buddy_allocator_malloc(&allocator, size);
    } else {                        // Altrimenti usa mmap
        void* mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (mem == MAP_FAILED) {
            printf("ERRORE: mmap fallito\n");
            return NULL;
        }
        printf("Allocato %d byte con mmap\n", size);
        return mem;
    }
}

//
void pseudo_free(void* mem, int size) {
    if (mem == NULL || size <= 0) {
        printf("ERRORE: puntatore nullo o dimensione non valida\n");
        return;
    }
    if(size < THRESHOLD) {
        buddy_allocator_free(&allocator, mem);
    } else {
        int res = munmap(mem, size);
        if(res == -1) {
            printf("ERRORE: munmap fallito\n");
            return;
        }
        printf("Deallocato %d byte con munmap\n", size);
    }
}
