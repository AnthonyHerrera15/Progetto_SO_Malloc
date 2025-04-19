#pragma once
#include "buddy_allocator.c"
#include <math.h>

#define MEMORY_SIZE 1024*1024   // dimensione della memoria gestita
#define THRESHOLD 1024          // dimensione massima per cui usare il buddy allocator (1/4 di una pagina(4096))
#define BUDDY_LEVELS 7          // numero di livelli del buddy allocator
#define BITMAP_SIZE (1 << BUDDY_LEVELS)-1               // dimensione della bitmap 
#define MIN_BUCKET_SIZE THRESHOLD/(1<<(BUDDY_LEVELS-1)) // =16 dimensione minima del blocco di memoria

void pseudo_init();
void* pseudo_malloc(Buddyallocator *allocator, int size);
void pseudo_free(Buddyallocator *allocator, void *ptr);
