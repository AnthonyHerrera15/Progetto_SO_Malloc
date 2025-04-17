#pragma once
#include "bitmap.c"

#define MAX_LEVELS 16

typedef struct{
    char* memory;           //puntatore alla memoria
    int memory_size;        //dimensione della memoria gestita
    int min_bucket_size;    //dimensione minima del blocco di memoria
    int num_levels;    
    Bitmap bitmap;
} BuddyAllocator;

// funzione che inizializza un BuddyAllocator ed effettua il setup della bitmap
void buddy_allocator_init(BuddyAllocator* allocator, char* memory, int mem_size, int min_bucket_size, char* bm_buffer, int bm_size);

// funzione che alloca la memoria richiesta
void buddy_allocator_malloc(BuddyAllocator* allocator, int size);

// funzione che libera la memoria allocata
void buddy_allocator_free(BuddyAllocator* allocator, void* ptr);

//funzioni ausiliari (prese dalle slides)
int buddyIdx(int idx);
int parentIdx(int idx);
int first_in_level(int level);

//funzioni ausiliari bitmap
void aggiorno_parenti(Bitmap* bitmap, int idx, int status);
void aggiorno_figli(Bitmap* bitmap, int idx, int status);

