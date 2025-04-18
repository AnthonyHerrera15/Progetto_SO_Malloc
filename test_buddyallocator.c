#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "buddy_allocator.c"

#define MEMORY_SIZE 256
#define MIN_BUCKET_SIZE 16
#define BUDDY_LEVELS 5
#define BITMAP_SIZE 32

int main(){
    char memory[MEMORY_SIZE];
    char bitmap_buffer[BITMAP_SIZE];

    BuddyAllocator allocator; 

    // Test errori di inizializzazione
    buddy_allocator_init(NULL, memory, MEMORY_SIZE, MIN_BUCKET_SIZE, bitmap_buffer, BITMAP_SIZE);
    buddy_allocator_init(&allocator, NULL, MEMORY_SIZE, MIN_BUCKET_SIZE, bitmap_buffer, BITMAP_SIZE);
    buddy_allocator_init(&allocator, memory, MEMORY_SIZE, MIN_BUCKET_SIZE, NULL, BITMAP_SIZE);
    printf("\n");

    // Test di inizializzazione
    buddy_allocator_init(&allocator, memory, MEMORY_SIZE, MIN_BUCKET_SIZE, bitmap_buffer, BITMAP_SIZE);

    printf("Bitmap prima dell'allocazione:\n");
    bitmap_print(&allocator.bitmap);
    
    // Test di allocazione di 8 byte (+ sizeof(int))
    void*ptr1 = buddy_allocator_malloc(&allocator, 8);
    printf("Bitmap dopo tentativo di allocare 8 byte (+ sizeof(int)):\n");
    bitmap_print(&allocator.bitmap);

    // Test di allocazione di 16 byte (+ sizeof(int))
    void*ptr2 = buddy_allocator_malloc(&allocator, 16);
    printf("Bitmap dopo tentativo di allocare 16 byte (+ sizeof(int)):\n");
    bitmap_print(&allocator.bitmap);
    
    // Test di allocazione di 32 byte (+ sizeof(int))
    void*ptr3 = buddy_allocator_malloc(&allocator, 32);
    printf("Bitmap dopo tentativo di allocare 32 byte (+ sizeof(int)):\n");
    bitmap_print(&allocator.bitmap);

    // Test di allocazione di 64 byte (+ sizeof(int))
    void*ptr4 = buddy_allocator_malloc(&allocator, 64);
    printf("Bitmap dopo tentativo di allocare 64 byte (+ sizeof(int)):\n");
    bitmap_print(&allocator.bitmap);

    // Test di allocazione di 128 byte (+ sizeof(int))
    void*ptr5 = buddy_allocator_malloc(&allocator, 128);
    printf("Bitmap dopo tentativo di allocare 128 byte (+ sizeof(int)):\n");
    bitmap_print(&allocator.bitmap);
    printf("\n");

    // Test errori di deallocazione
    buddy_allocator_free(NULL, ptr1);
    buddy_allocator_free(&allocator, NULL);
    
    int temp = *((int*)ptr1-sizeof(int));       //salvo l'indice del blocco di memoria
    *((int*)ptr1-sizeof(int)) = -1;             //modifico l'indice del blocco (a -1) per simulare un errore
    buddy_allocator_free(&allocator, ptr1);     //provo la free per vedere se da errore
    
    *((int*)ptr1-sizeof(int)) = 16;             //inserisco indice di un blocco libero
    buddy_allocator_free(&allocator, ptr1);     //provo la free per vedere se da errore
    *((int*)ptr1-sizeof(int)) = temp;           //ripristino l'indice corretto del blocco di memoria

    // Test di deallocazione di ptr1(8 byte + sizeof(int))      Qui possiamo vedere il merge
    buddy_allocator_free(&allocator, ptr1);
    printf("Bitmap dopo tentativo deallocazione di ptr1 (8 byte + sizeof(int)):\n");
    bitmap_print(&allocator.bitmap);
    
    // Test di deallocazione di ptr2(16 byte + sizeof(int))     Qui possiamo vedere il merge e l'aggiornamento dei figli
    buddy_allocator_free(&allocator, ptr2);
    printf("Bitmap dopo tentativo deallocazione di ptr2 (16 byte + sizeof(int)):\n");
    bitmap_print(&allocator.bitmap);

    // Test di deallocazione di ptr4(64 byte + sizeof(int))     
    buddy_allocator_free(&allocator, ptr4);
    printf("Bitmap dopo tentativo deallocazione di ptr4 (64 byte + sizeof(int)):\n");
    bitmap_print(&allocator.bitmap);

    /* Pulisco la bitmap
    clear_allbits(&allocator.bitmap);
    printf("Bitmap dopo la pulizia:\n");
    bitmap_print(&allocator.bitmap);*/
    
    return 0;
}