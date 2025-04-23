#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pseudo_malloc.c"

int main(){
    pseudo_init();
    printf("Allocatore inizializzato con successo\n\n");

    // Test errore di allocazione
    printf("Tentativo di allocare memoria con dimensione negativa\n");
    void* ptr = pseudo_malloc(-1);
    printf("Tentativo di allocare memoria con dimensione zero\n");
    void* pt = pseudo_malloc(0);

    // Test di piccole allocazioni
    printf("\nTEST DI ALLOCAZIONI DI PICCOLE DIMENSIONI\n");

    void* ptr1 = pseudo_malloc(100);
    if(ptr1 == NULL) { printf("Allocazione di 100 byte fallita\n");} 
    else {
        printf("Allocato 100 byte con successo\n");
        buddy_bitmap_print(&allocator.bitmap, allocator.num_levels);
    }

    void* ptr2 = pseudo_malloc(512);
    if(ptr2 == NULL) { printf("Allocazione di 512 byte fallita\n");} 
    else {
        printf("Allocato 512 byte con successo\n");
        buddy_bitmap_print(&allocator.bitmap, allocator.num_levels);
    }

    void* ptr3 = pseudo_malloc(1020);
    if(ptr3 == NULL) { printf("Allocazione di 1020 byte fallita\n");} 
    else {
        printf("Allocato 1020 byte con successo\n");
        buddy_bitmap_print(&allocator.bitmap, allocator.num_levels);
    }

    // Test errore di deallocazione con buddy allocator
    printf("\nTentativo di deallocare un puntatore nullo\n");
    pseudo_free(NULL, 1);
    printf("\nTentativo di deallocare un puntatore non allocato\n");
    pseudo_free(ptr, 1);
    printf("\nTentativo di deallocare un puntatore con dimensione negativa\n");
    pseudo_free(ptr1, -1);

    // Test di deallocazioni di piccole allocazioni
    printf("\nTEST DI DEALLOCAZIONE DI PICCOLE DIMENSIONI\n");

    pseudo_free(ptr1, 100);
    buddy_bitmap_print(&allocator.bitmap, allocator.num_levels);

    pseudo_free(ptr2, 512);
    buddy_bitmap_print(&allocator.bitmap, allocator.num_levels);

    pseudo_free(ptr3, 1020);
    buddy_bitmap_print(&allocator.bitmap, allocator.num_levels);

    printf("\nDeallocazioni effettuate con successo\n\n");

    // Test di grandi allocazioni
    printf("TEST DI ALLOCAZIONI DI GRANDI DIMENSIONI\n");

    void* ptr4 = pseudo_malloc(2048);
    if(ptr4 == NULL) { printf("Allocazione di 2048 byte fallita\n");} 
    else {
        printf("Allocato 2048 byte con successo\n");
        buddy_bitmap_print(&allocator.bitmap, allocator.num_levels);
    }

    void* ptr5 = pseudo_malloc(1021);                                   //Test di allocazione sul limite del Threshold
    if(ptr5 == NULL) { printf("Allocazione di 1024 byte fallita\n");} 
    else {
        printf("Allocato 1021 byte con successo\n");
        buddy_bitmap_print(&allocator.bitmap, allocator.num_levels);
    }

    // Test errore di deallocazione con mmap
    printf("\nTentativo di deallocare con munmap una zona non allocata con mmap\n");
    pseudo_free(ptr1, 2000);

    // Test di deallocazione di grandi allocazioni
    printf("\nTEST DI DEALLOCAZIONE DI GRANDI DIMENSIONI\n");
    pseudo_free(ptr4, 2048);
    pseudo_free(ptr5, 1021);
    printf("Deallocazioni effettuate con successo\n");

    return 0;
}