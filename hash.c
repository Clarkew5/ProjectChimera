#include "hash.h"

struct Hash *createHash(size_t size){
    struct Hash *hash = calloc(1, sizeof(struct Hash));
    if (hash == NULL)
        return NULL;
    hash->size = size;
    hash->entries = calloc(size, sizeof(struct Entry *));
    if (hash->entries == NULL){
        printf("Calloc failed. No memory for hash.\n");
        return NULL;
    }

    return hash;
}

int destroyHash(struct Hash *hash){
    struct Entry *p;
    struct Entry *pPrev;
    for (size_t i = 0; i < hash->size; i++){
        pPrev = *(hash->entries + i);
        if (pPrev == NULL){
            free(pPrev);
            continue;
        }
        p = pPrev->next;
        while (p != NULL){
            free(pPrev);
            pPrev = p;
            p = p->next;
        }
        free(pPrev);
        free(p);
    }
    free(hash->entries);
    free(hash);
    return 0;
}

int hashFunction(struct Hash *hash){
    unsigned int index = 0;
    for (int i = 0; i < 6; i++){   
        index += ARIMAABOARD.gold[i] % hash->size;
        index += ARIMAABOARD.silver[i] % hash->size;
    }
    return index % hash->size;
}

int insertEntry(uint16_t *move, struct Hash *hash){
    int index = hashFunction(hash);
    struct Entry *p = *(hash->entries + index);
    if (p == NULL){
        p = calloc(1, sizeof(struct Entry));
        for (int i =0 ; i < 6; i++){
            *(p->gold + i) = ARIMAABOARD.gold[i];
            *(p->silver + i) = ARIMAABOARD.silver[i];
        }
        for (int i = 0; i < 4; i++)
            *(p->move + i) = move[i];
        (*(hash->entries + index)) = p;
    }
    else{
        while(p->next != NULL){
            if (p->gold[0] == ARIMAABOARD.gold[0] &&
                p->gold[1] == ARIMAABOARD.gold[1] &&
                p->gold[2] == ARIMAABOARD.gold[2] &&
                p->gold[3] == ARIMAABOARD.gold[3] &&
                p->gold[4] == ARIMAABOARD.gold[4] &&
                p->gold[5] == ARIMAABOARD.gold[5] &&
                p->silver[0] == ARIMAABOARD.silver[0] &&
                p->silver[1] == ARIMAABOARD.silver[1] &&
                p->silver[2] == ARIMAABOARD.silver[2] &&
                p->silver[3] == ARIMAABOARD.silver[3] &&
                p->silver[4] == ARIMAABOARD.silver[4] &&
                p->silver[5] == ARIMAABOARD.silver[5])
                    return 1;
            p = p->next;
        }
        p->next = calloc(1, sizeof(struct Entry));
        p = p->next;
        for (int i = 0; i < 6; i++){
            p->gold[i] = ARIMAABOARD.gold[i];
            p->silver[i] = ARIMAABOARD.silver[i];
        }
        for (int i = 0; i < 4; i++)
            p->move[i] = move[i];
    }
    return 0;
}

int printTable(struct Hash *hash){
    for (int i = 0; i < hash->size; i++){
        struct Entry *p = *(hash->entries + i);
        while(p != NULL){
            for (int i = 0; i < 4; i++){
                printMove(p->move[i]);
            }
            printf(">");
            p = p->next;
        }
        printf("No Entry\n");
    }
    return 0;
}
