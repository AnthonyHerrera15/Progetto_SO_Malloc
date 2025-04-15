#pragma once
#include "bitmap.c"

typedef struct{
    char* memory;           //puntatore alla memoria
    int memory_size;        //dimensione della memoria gestita
    int min_bucket_size;    //dimensione minima del blocco di memoria
    int num_levels;    
    Bitmap bitmap;
} BuddyAllocator;

// funzione che inizializza un BuddyAllocator ed effettua il setup della bitmap
void buddy_allocator_init(BuddyAllocator* allocator, char* memory, int memory_size, int min_bucket_size, char* bm_buffer, int bm_size);

// funzione che alloca la memoria richiesta
void buddy_allocator_malloc(BuddyAllocator* allocator, int size);
