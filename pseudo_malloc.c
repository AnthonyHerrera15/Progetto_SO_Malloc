#include "pseudo_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

char memory[MEMORY_SIZE]; // memoria gestita dal buddy allocator
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
    if (size <= THRESHOLD-(int)sizeof(int)) {         // Se la dimensione è minore di THRESHOLD, usa il buddy allocator
        return buddy_allocator_malloc(&allocator, size);
    } else 
    {                                          // Altrimenti usa mmap
        int total_size = size+sizeof(int);                        
        void* mem = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        if (mem == MAP_FAILED) {
            printf("ERRORE: mmap fallito\n");
            return NULL;
        }
        *(int*)mem = size;          // memorizzo la dimensione all'inizio del blocco di memoria
        mem += sizeof(int);         // sposto il puntatore oltre la dimensione

        printf("\nAllocato %d byte con mmap\n", *((int*)(mem-sizeof(int))));
        return mem;
    }
}

// Dealloca un blocco di memoria
void pseudo_free(void*mem) {
    if (mem == NULL) {
        printf("ERRORE: puntatore nullo\n");
        return;
    }
    // Controllo se il puntatore è all'interno della memoria gestita dal buddy allocator
    char* buddy_memory_start = allocator.memory;
    char* buddy_memory_end = buddy_memory_start + MEMORY_SIZE;

    if (mem >= (void*)buddy_memory_start && mem < (void*)buddy_memory_end ){
        buddy_allocator_free(&allocator, mem);
    } else 
    {                                       // Altrimenti usa munmap 
        int* ptr_size = (int*)(mem-sizeof(int));    // sposto il puntatore per accedere alla dimensione
        int size = *ptr_size;                       // prendo la dimensione memorizzata all'inizio del blocco 

        if (size <= 0) {
            printf("ERRORE: dimensione non valida\n");
            return;
        }
        // Dealloca la memoria usando munmap           
        int res = munmap(ptr_size, size);                
        if (res == -1) {
            printf("ERRORE: munmap fallito\n");
            return;
        }
        printf("Deallocato %d byte con munmap\n", size);
    }
}
