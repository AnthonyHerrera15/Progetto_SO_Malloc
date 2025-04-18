#include "bitmap.h"
#include <stdio.h>
#include <math.h>

int bitmap_getBytes(int n_bits) {
    return (n_bits/8) + ((n_bits % 8)!=0);
}

void bitmap_init(Bitmap* bitmap, int n_bits, char* buffer) {
    bitmap->bits = buffer;
    bitmap->n_bits = n_bits;
    for(int i=0; i<bitmap_getBytes(n_bits); i++) { //inizializzo tutti i bit a 0
        bitmap->bits[i] = 0;
    }
}

void bitmap_set(Bitmap* bitmap, int bit_num, int value){
    if(bit_num<0 || bit_num >= bitmap->n_bits) {    //controllo che il bit_num sia valido
        printf("ERRORE(set in %d): indice del bit non valido\n",bit_num);
        return;  
    }
    int byte_index = bit_num/8;     //indice del byte dentro il buffer
    int bit_index = bit_num%8;      //indice del bit dentro il byte
    if(value) {
        bitmap->bits[byte_index] |= (1 << bit_index);   //imposto il bit richiesto a 1
    } else {
        bitmap->bits[byte_index] &= ~(1 << bit_index);  //imposto il bit richiesto a 0
    }
}

int bitmap_get(Bitmap* bitmap, int bit_num){
    if(bit_num<0 || bit_num >= bitmap->n_bits) {    //controllo che il bit_num sia valido
        printf("ERRORE(get in %d): indice del bit non valido\n",bit_num);
        return -1;  
    }
    int byte_index = bit_num/8;     //indice del byte dentro il buffer
    int bit_index = bit_num%8;      //indice del bit dentro il byte
    int ris = (bitmap->bits[byte_index] & (1 << bit_index))!=0; //restituisco il valore del bit richiesto
    return ris;
}

void bitmap_print(Bitmap* bitmap) {
    if(!bitmap || !bitmap->bits) {
        printf("Bitmap non inizializzato\n");
        return;
    }
    printf("Bitmap: ");
    int p2 = 0;
    int cont = 0;
    for(int i=0; i<bitmap->n_bits; i++) {
        
        if(cont == 1<<p2) {
            printf(" ");
            p2 ++;
            cont = 0;
        }
        printf("%d",bitmap_get(bitmap, i));
        cont++;
    }
    printf("\n");
}