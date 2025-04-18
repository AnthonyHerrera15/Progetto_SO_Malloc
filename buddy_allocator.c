#include "buddy_allocator.h"
#include <math.h>
#include <stdio.h>

// trova il livello del nodo idx
int levelIdx(int idx) {
    int level = 0;
    while ((1 << level) - 1 <= idx) {
        level++;
    }
    return level - 1;
}

// trova l'indice del buddy del nodo idx
int buddyIdx(int idx){
    if(idx == 0) return 0;
    if(idx%2 == 0) return idx - 1;
    return idx + 1;
}

// trova l'indice del padre del nodo idx
int parentIdx(int idx){
    if(idx==0) return -1;
    return (int)(idx-1)/2;
}

// trova l'indice del primo nodo di un certo livello
int first_in_level(int level){
    return (1<<level)-1;
}

// trova l'indice del nodo idx all'interno del livello
int offset_in_level(int level, int idx){
    return (idx - first_in_level(level));
}                                           //ALLA FINE NON L'HO USATA

void buddy_allocator_init(BuddyAllocator* allocator, char* memory, int mem_size, int min_bucket_size, char* bm_buffer, int bm_size) {
    // controllo se i parametri sono validi
    if (!allocator || !memory || mem_size <= 0 || min_bucket_size <= 0 || !bm_buffer || bm_size <= 0) {
        printf("Parametri non validi per inizializzare un BuddyAllocator\n");
        return;
    }

    int test_mem_size=1;
    while(test_mem_size<<1 <= mem_size){         //trovo la potenza di 2 più vicina a mem_size
        test_mem_size<<=1;
    }

    int test_minb_size=1;
    while(test_minb_size<<1 <= min_bucket_size){  //trovo la potenza di 2 più vicina a min_bucket_size
        test_minb_size<<=1;
    }
    
    int levels = 0;
    int size = test_mem_size;
    while(size >= test_minb_size) {             
        size >>= 1;
        levels++;
    }
    if(levels > MAX_LEVELS) {
        printf("ERRORE: numero di livelli non valido\n");
        return;
    }

    allocator->memory = memory;                 //memoria da gestire
    allocator->memory_size = test_mem_size;     //pongo la dimensione della memoria gestita uguale alla dimensione trovata
    allocator->min_bucket_size = test_minb_size;//pongo la dimensione minima del blocco di memoria uguale alla dimensione trovata
    allocator->num_levels = levels;             //pongo num_levels uguale al numero di livelli trovato

    int n_bits = (1<<levels)-1;         //numero di bit necessari nella bitmap
    if(n_bits > bm_size) {
        printf("ERRORE: buffer bitmap troppo piccolo\n");
        return;
    }
    bitmap_init(&allocator->bitmap, n_bits, bm_buffer);
}

void aggiorno_parenti(Bitmap* bitmap, int idx, int status){
    bitmap_set(bitmap, idx, status);                //segnalo il nodo come occupato o libero
    int parent = parentIdx(idx);
    if(parent != -1) {
        aggiorno_parenti(bitmap, parent, status);   //se non sono alla radice segnalo il padre
    }
}

void aggiorno_figli(Bitmap* bitmap, int idx, int status){
    if(idx <= bitmap->n_bits-1){              
        bitmap_set(bitmap, idx, status);            //segnalo il nodo come occupato o libero
        aggiorno_figli(bitmap, (2*idx)+1, status);    //segnalo il figlio sinistro
        aggiorno_figli(bitmap, (2*idx)+2, status);    //segnalo il figlio destro
    }
}

void* buddy_allocator_malloc(BuddyAllocator* allocator, int size) {
    printf("\nRichiesta di %d byte \n", size);
    // controllo se i parametri sono validi
    if (!allocator || !allocator->memory || size <= 0 || size > allocator->memory_size) {
        printf("Parametri non validi per la malloc\n");
        return NULL;
    }

    // aggiungo lo spazio per il buddy block header dove memorizzo l'indice del blocco
    size+=sizeof(int); 

    // calcolo il livello necessario per la richiesta di memoria
    int level = allocator->num_levels - 1;
    int bucket_size = allocator->min_bucket_size;
    while (size > bucket_size) {            //se la richiesta di memoria è maggiore della dimensione minima
        bucket_size <<= 1;                  // del blocco di memoria raddoppio la dimensione del blocco e 
        level--;                            // decremento di un livello e riprovo 
    }
    // cerco il primo blocco di memoria libero
    int idx = first_in_level(level);
    
    while (idx >=0 && idx < first_in_level(level + 1)) { //fino a quando non arrivo alla fine del livello
        
        if (bitmap_get(&allocator->bitmap, idx) == 0) {     //se il blocco è libero
            //propago l'aggiornamento fino alla radice
            aggiorno_parenti(&allocator->bitmap, idx, 1);
            
            //propago l'aggiornamento ai nodi figli
            aggiorno_figli(&allocator->bitmap, idx, 1);
            
            // calcolo l'indirizzo del blocco di memoria
            int* block_addr = (int*)(allocator->memory + (idx - first_in_level(level)) * bucket_size);
            *block_addr = idx; // memorizzo l'indice del blocco di memoria

            printf("Allocato %d byte a livello %d, indice %d\n", bucket_size, level, idx);

            return (void*)(block_addr + sizeof(int)); // restituisco l'indirizzo del blocco di memoria
        }
        idx++;
    }

    // nessun blocco di memoria libero trovato
    printf("ERRORE: memoria esaurita\n");
    return NULL; 
}

void buddy_allocator_free(BuddyAllocator* allocator, void* ptr){
    // controllo se i parametri sono validi
    if (!allocator || !allocator->memory || !ptr) {
        printf("Parametri non validi per la free\n");
        return;
    }
    
    // calcolo l'indirizzo del blocco di memoria e prendo l'indice nell'header
    int* block_addr = ((int*)ptr-sizeof(int));
    int idx = *block_addr;

    printf("\nRichiesta di deallocazione del blocco di memoria con indice %d\n", idx);

    // controllo se l'indice è valido
    if(idx < 0 || idx >= allocator->bitmap.n_bits) {
        printf("ERRORE: indice del blocco di memoria non valido\n");
        return;
    }

    // controllo se il blocco di memoria è già libero
    if (bitmap_get(&allocator->bitmap, idx) == 0) { 
        printf("ERRORE: blocco di memoria già libero\n");
        return;
    }
    
    // segnalo il nodo e i suoi figli come liberi
    aggiorno_figli(&allocator->bitmap, idx, 0); //segnalo il blocco di memoria come libero

    // provo a fare il merging con il buddy
    int buddy = buddyIdx(idx);
    while(bitmap_get(&allocator->bitmap, buddy) == 0 && idx>0) {
        idx = parentIdx(idx);
        bitmap_set(&allocator->bitmap, idx, 0);
        buddy = buddyIdx(idx);          //calcolo il buddy del padre
    }
    printf("Deallocato con successo il blocco di memoria con indice %d\n", (*block_addr));
}

void clear_allbits(Bitmap* bitmap) {
    for(int i=0; i<bitmap->n_bits; i++) {
        bitmap->bits[i] = 0;
    }
}

void buddy_allocator_print(BuddyAllocator* allocator) {
    if (!allocator || !allocator->memory) {
        printf("BuddyAllocator non inizializzato\n");
        return;
    }
    printf("BuddyAllocator: \n");
    printf("Memory size: %d\n", allocator->memory_size);
    printf("Min bucket size: %d\n", allocator->min_bucket_size);
    printf("Num levels: %d\n", allocator->num_levels);
    bitmap_print(&allocator->bitmap);
}