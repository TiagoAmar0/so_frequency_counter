#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdint.h>

#include "counter.h"
#include "debug.h"
#include "table.h"
#include "memory.h"


// Tries to open a file in a given mode and returns the file pointer
FILE *openFile(char *filename, char* mode){
    FILE *f;

    f = fopen(filename, mode);
    if (f == NULL) {
        fprintf(stderr, "*** Invalid Filename %s.", filename);
        exit(1);
    }

    return f;
}

void readFile(char *filename, int mode){
    FILE *fptr = NULL;

    fptr = openFile(filename, "rb");

    switch (mode) {
        case 1:
            readBytes(fptr, filename);
            break;

        case 2:
            readDuoBytes(fptr, filename);
            break;
        
        case 4:
            readQuadBytes(fptr, filename);
            break;
    }
}

void readBytes(FILE *fptr, char *filename){
        uint8_t byte;
        int elements_read;

        HashTable *hashTable = NULL;

        unsigned int *combinations = NULL;
        unsigned int insertedCombinations = 0;

        
        hashTable = createHashTable(BYTE_HASH_TABLE_SIZE, BYTE_HASH_VALUE);
        if (hashTable == NULL) {
            exit(1);
        }
        
        while(!feof(fptr)){
            elements_read = fread(&byte, sizeof(byte), 1, fptr);
            if(elements_read == 1){
                combinations = insertInHashTable(hashTable, byte, combinations, &insertedCombinations);
            }
        }


        qsort(combinations, insertedCombinations, sizeof(unsigned int), compareNumbers);
        printHashtable(filename, hashTable, combinations, insertedCombinations, "byte");
        freeHashTable(hashTable);
        free(combinations);
}

void readDuoBytes(FILE *fptr, char *filename){
        uint16_t byte;
        int elements_read;

        HashTable *hashTable = NULL;

        unsigned int *combinations = NULL;
        unsigned int insertedCombinations = 0;

        
        hashTable = createHashTable(DUO_BYTE_HASH_TABLE_SIZE, DUO_BYTE_HASH_VALUE);
        if (hashTable == NULL) {
            exit(1);
        }
        
        while(!feof(fptr)){
            elements_read = fread(&byte, sizeof(byte), 1, fptr);
            if(elements_read == 1){
                combinations = insertInHashTable(hashTable, byte, combinations, &insertedCombinations);
            }
        }


        qsort(combinations, insertedCombinations, sizeof(unsigned int), compareNumbers);
        printHashtable(filename, hashTable, combinations, insertedCombinations, "bi-byte");
        freeHashTable(hashTable);
        free(combinations);
}

void readQuadBytes(FILE *fptr, char *filename){
        uint32_t byte;
        int elements_read;

        HashTable *hashTable = NULL;

        unsigned int *combinations = NULL;
        unsigned int insertedCombinations = 0;

        
        hashTable = createHashTable(QUAD_BYTE_HASH_TABLE_SIZE, QUAD_BYTE_HASH_VALUE);
        if (hashTable == NULL) {
            exit(1);
        }
        
        while(!feof(fptr)){
            elements_read = fread(&byte, sizeof(byte), 1, fptr);
            if(elements_read == 1){
                combinations = insertInHashTable(hashTable, byte, combinations, &insertedCombinations);
            }
        }

        // Needs fixing for quad bytes values
        qsort(combinations, insertedCombinations, sizeof(unsigned int), compareNumbers);
        
        printHashtable(filename, hashTable, combinations, insertedCombinations, "quad-byte");
        freeHashTable(hashTable);
        free(combinations);
}

void readBytes(FILE *fptr, char *filename, unsigned int *values){
        uint8_t byte;
        int elements_read;

        HashTable *hashTable = NULL;

        unsigned int *combinations = NULL;
        unsigned int insertedCombinations = 0;

        
        hashTable = createHashTable(BYTE_HASH_TABLE_SIZE, BYTE_HASH_VALUE);
        if (hashTable == NULL) {
            exit(1);
        }
        
        while(!feof(fptr)){
            elements_read = fread(&byte, sizeof(byte), 1, fptr);
            if(elements_read == 1){
                combinations = insertInHashTable(hashTable, byte, combinations, &insertedCombinations);
            }
        }


        qsort(combinations, insertedCombinations, sizeof(unsigned int), compareNumbers);
        printHashtable(filename, hashTable, combinations, insertedCombinations, "byte");
        freeHashTable(hashTable);
        free(combinations);
}

// Qsort comparison function
int compareNumbers(const void * a, const void * b){
    return (*(unsigned int*)a - *(unsigned int*)b);
}