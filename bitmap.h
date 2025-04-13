#pragma once
#include <stdint.h>

typedef struct{
    char* bits;
    int n_bits;
} Bitmap;

// funzione che inizializza un bitmap 
void bitmap_init(Bitmap* bitmap, int n_bits, char* buffer);

// funzione che calcola e restituisce il numero di byte necessari per memorizzare i bits
int bitmap_getBytes(int n_bits);

// funzione che imposta un bit specifico nella bitmap ad un valore: 0 o 1. 
void bitmap_set(Bitmap* bitmap, int bit_num, int value);

// funzione che restituisce il valore di un bit specifico nella bitmap (0 o 1)
int bitmap_getBit(Bitmap* bitmap, int bit_num);
