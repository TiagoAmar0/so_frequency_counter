#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdint.h>

#include "counter.h"
#include "debug.h"
#include "table.h"
#include "memory.h"
#include "files.h"



void readFile(char *filename, Info info){
    FILE *fptr = NULL;

    fptr = openFile(filename, "rb");

    if(fptr == NULL){
        printf("\nERROR:'%s': CANNOT PROCCESS FILE\n-----\n", filename);
        return;
    }

    // Get file size
    fseek(fptr, 0L, SEEK_END);
    info.file_size = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);

    switch (info.mode) {
        case 1:
            readBytes(fptr, filename, info);
            break;

        case 2:
            readDuoBytes(fptr, filename, info);
            break;
        
        case 4:
            readQuadBytes(fptr, filename, info);
            break;
    }

    fclose(fptr);
}

void readBytes(FILE *fptr, char *filename, Info info){
        uint8_t byte;
        int elements_read;

        HashTable *hashTable = NULL;

        unsigned int *combinations = NULL;
        unsigned int inserted_combinations = 0;
        int byte_position = 0;

        
        hashTable = createHashTable(BYTE_HASH_TABLE_SIZE, BYTE_HASH_VALUE);
        if (hashTable == NULL) {
            exit(1);
        }
        
        while(!feof(fptr)){
            elements_read = fread(&byte, sizeof(byte), 1, fptr);
            if(elements_read == 1){
                if(info.discrete){
                    byte_position = findInLongArray(byte, info.discrete_values, info.discrete_values_total);
                    if(byte_position != NOT_FOUND){
                        combinations = insertInHashTable(hashTable, byte, combinations, &inserted_combinations);
                    }
                } else {
                    combinations = insertInHashTable(hashTable, byte, combinations, &inserted_combinations);
                }
            }
        }


        qsort(combinations, inserted_combinations, sizeof(unsigned int), compareNumbers);
        
        outputData(filename, hashTable, info, combinations, inserted_combinations, "byte");
        freeHashTable(hashTable);
        free(combinations);
}

void readDuoBytes(FILE *fptr, char *filename, Info info){
        uint16_t duo_byte;
        int elements_read;

        HashTable *hashTable = NULL;

        unsigned int *combinations = NULL;
        unsigned int inserted_combinations = 0;
        int byte_position = 0;
        
        hashTable = createHashTable(DUO_BYTE_HASH_TABLE_SIZE, DUO_BYTE_HASH_VALUE);
        if (hashTable == NULL) {
            exit(1);
        }
        
        while(!feof(fptr)){
            elements_read = fread(&duo_byte, sizeof(duo_byte), 1, fptr);
            if(elements_read == 1){
                if(info.discrete){
                    byte_position = findInLongArray(duo_byte, info.discrete_values, info.discrete_values_total);
                    if(byte_position != NOT_FOUND){
                        combinations = insertInHashTable(hashTable, duo_byte, combinations, &inserted_combinations);
                    }
                } else {
                    combinations = insertInHashTable(hashTable, duo_byte, combinations, &inserted_combinations);
                }
            }
        }


        qsort(combinations, inserted_combinations, sizeof(unsigned int), compareNumbers);

        outputData(filename, hashTable, info, combinations, inserted_combinations, "bi-byte");

        freeHashTable(hashTable);
        free(combinations);
}

void readQuadBytes(FILE *fptr, char *filename, Info info){
        uint32_t quad_byte;
        int elements_read;
        int byte_position;

        HashTable *hashTable = NULL;

        unsigned int *combinations = NULL;
        unsigned int inserted_combinations = 0;

        
        hashTable = createHashTable(QUAD_BYTE_HASH_TABLE_SIZE, QUAD_BYTE_HASH_VALUE);
        if (hashTable == NULL) {
            exit(1);
        }
        
        while(!feof(fptr)){
            elements_read = fread(&quad_byte, sizeof(quad_byte), 1, fptr);
            if(elements_read == 1){
                if(info.discrete){
                    byte_position = findInLongArray(quad_byte, info.discrete_values, info.discrete_values_total);
                    if(byte_position != NOT_FOUND){
                        combinations = insertInHashTable(hashTable, quad_byte, combinations, &inserted_combinations);
                    }
                } else {
                    combinations = insertInHashTable(hashTable, quad_byte, combinations, &inserted_combinations);
                }
            }
        }

        // Needs fixing for quad bytes values
        qsort(combinations, inserted_combinations, sizeof(unsigned int), compareNumbers);
        outputData(filename, hashTable, info, combinations, inserted_combinations, "quad-byte");
        freeHashTable(hashTable);
        free(combinations);
}

void outputData(char *filename, HashTable *hashTable, Info info, unsigned int *combinations, unsigned int inserted_combinations, char* description){
    if(info.output){
        if(info.compact){
            printInFileCompact(filename, hashTable, info.output_target, info.file_size, combinations, inserted_combinations);
        } else {
            printInFile(filename, hashTable, info.output_target, info.file_size, combinations, inserted_combinations, description);
        }
    } else {
        if(info.compact){
            printHashtableCompact(filename, hashTable, info.file_size, combinations, inserted_combinations);
        } else {
            printHashtable(filename, hashTable, info.file_size, combinations, inserted_combinations, description);
        }
    }
}



// Qsort comparison function
int compareNumbers(const void * a, const void * b){
    unsigned int *value1 = (unsigned int*)a;
    unsigned int *value2 = (unsigned int*)b;

    if(*value1 == *value2){
        return 0;
    }

    if(*value1 >= *value2){
        return 1;
    }
    return -1;
}

// Retrieve position from array
unsigned int findInLongArray(unsigned int value, long *array, unsigned int total){
    for(unsigned int i=0; i < total; i++){
        if(value == (unsigned int) array[i]){
            return i;
        }
    }

    return NOT_FOUND;
}