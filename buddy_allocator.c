#include "buddy_allocator.h"

// trova il livello del nodo idx
int levelIdx(int idx){
    return (int)floor(log2(idx+1));
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
}

void buddy_allocator_init(BuddyAllocator* allocator, char* memory, int mem_size, int min_bucket_size, char* bm_buffer, int bm_size) {
    // controllo se i parametri sono validi
    if (!allocator || !memory || mem_size <= 0 || min_bucket_size <= 0 || !bm_buffer || bm_size <= 0) {
        printf("Parametri non validi per inizializzare un BuddyAllocator\n");
        return;
    }

    int test_mem_size=1;
    while(test_mem_size<<1 < mem_size){         //trovo la potenza di 2 più vicina a mem_size
        test_mem_size<<=1;
    }

    int test_minb_size=1;
    while(test_minb_size<<1 < min_bucket_size){  //trovo la potenza di 2 più vicina a min_bucket_size
        test_minb_size<<=1;
    }
    
    int levels = 0;
    int size = test_mem_size;
    while(size >= test_minb_size) {              // ?? (controllare se esce il numero di livelli giusto)
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
    bitmap_set(bitmap, idx, status);            //segnalo il nodo come occupato o libero
    aggiorna_figli(bitmap, (2*idx)+1, status);    //segnalo il figlio sinistro
    aggiorno_figli(bitmap, (2*idx)+2, status);    //segnalo il figlio destro
}

void buddy_allocator_malloc(BuddyAllocator* allocator, int size) {
    // controllo se i parametri sono validi
    if (!allocator || !allocator->memory || size <= 0 || size > allocator->memory_size) {
        printf("Parametri non validi per la malloc\n");
        return;
    }

    // aggiungo la lo spazio per il buddy block header dove memorizzo l'indice del blocco
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
    while (idx < allocator->bitmap.n_bits) {
        if (bitmap_get(&allocator->bitmap, idx) == 0) {     //se il blocco è libero

            //propago l'aggiornamento fino alla radice
            aggiorna_parenti(allocator->bitmap, idx, 1);

            //propago l'aggiornamento ai nodi figli
            aggiorna_figli(allocator->bitmap, idx, 1);

            // calcolo l'indirizzo del blocco di memoria
            int* block_addr = (int*)(allocator->memory + (idx - first_in_level(level)) * bucket_size);
            *block_addr = idx; // memorizzo l'indice del blocco di memoria

            printf("Allocato %d bit a livello %d, indice %d\n", bucket_size, level, idx);

            return (void*)block_addr + sizeof(int); // restituisco l'indirizzo del blocco di memoria
        }
        idx++;
    }

    // nessun blocco di memoria libero trovato
    printf("ERRORE: memoria esaurita\n");
    return NULL; 
}