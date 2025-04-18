#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
//#include "bitmap.h"
//#include "bitmap.c"
//#include "buddy_allocator.h"
#include "buddy_allocator.c"

#define MEMORY_SIZE 256
#define MIN_BUCKET_SIZE 16
#define BUDDY_LEVELS 5
#define BITMAP_SIZE 32

int main(){
    char memory[MEMORY_SIZE];
    char bitmap_buffer[BITMAP_SIZE];

    BuddyAllocator allocator; 
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
    
    // Test di deallocazione
    //buddy_allocator_free(&allocator, ptr1);
    //buddy_allocator_free(&allocator, ptr2);
    
    // Test di stampa della bitmap
    //bitmap_print(&allocator.bitmap);
    
    return 0;
}