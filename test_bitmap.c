#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "bitmap.c"

void test_bitmap_getBytes(){
    assert(bitmap_getBytes(0) == 0);
    assert(bitmap_getBytes(1) == 1);
    assert(bitmap_getBytes(8) == 1);
    assert(bitmap_getBytes(9) == 2);
    assert(bitmap_getBytes(16) == 2);
    assert(bitmap_getBytes(17) == 3);
    assert(bitmap_getBytes(24) == 3);
    assert(bitmap_getBytes(25) == 4);
    printf("test dimensione byte andato a buon fine\n");
}

void test_bitmap_init(){
    char buffer[3];   
    Bitmap bitmap; 

    bitmap_init(&bitmap, 24, buffer); 

    for(int i=0; i<bitmap_getBytes(24); i++) {
        assert(bitmap.bits[i] == 0);
    }
    printf("test bitmap_init andato a buon fine\n");
}

int main(){

    test_bitmap_getBytes();
    test_bitmap_init();

    // Test bitmap_set and bitmap_get
    char buffer[2];
    Bitmap bitmap;
    bitmap_init(&bitmap, 16, buffer); 

    bitmap_set(&bitmap, 0, 1); 
    assert(bitmap_get(&bitmap, 0) == 1);
    
    bitmap_set(&bitmap, 1, 0);
    assert(bitmap_get(&bitmap, 1) == 0);

    bitmap_set(&bitmap, 7, 1);
    assert(bitmap_get(&bitmap, 7) == 1);

    bitmap_set(&bitmap, 8, 0);
    assert(bitmap_get(&bitmap, 8) == 0);

    bitmap_set(&bitmap, 15, 1);
    assert(bitmap_get(&bitmap, 15) == 1);

    bitmap_set(&bitmap, 16, 1);             // Test sul limite
    assert(bitmap_get(&bitmap, 16) == -1);  

    bitmap_print(&bitmap);

    printf("test bitmap_set and bitmap_get andato a buon fine\n");
    printf("TUTTI I TEST PASSATI CON SUCCESSO!\n");

    return 0;
}