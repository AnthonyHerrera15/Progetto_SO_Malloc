#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <math.h>
#include "bitmap.c"

int main(){
    int n_bits = 16; 
    char buffer[bitmap_getBytes(n_bits)];   // buffer per memorizzare i bit
    Bitmap bitmap; 

    bitmap_init(&bitmap, n_bits, buffer); 

    bitmap_set(&bitmap, 0, 1);
    bitmap_set(&bitmap, 1, 0);
    bitmap_set(&bitmap, 2, 1);
    bitmap_set(&bitmap, 3, 0);
    bitmap_set(&bitmap, 4, 1);
    bitmap_set(&bitmap, 5, 0);
    bitmap_set(&bitmap, 6, 1);
    bitmap_set(&bitmap, 12, 1);

    bitmap_print(&bitmap);

    return 0;
}