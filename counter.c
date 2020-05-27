#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "counter.h"
#include "debug.h"
#include "table.h"
#include "memory.h"
#include "files.h"

#define ERR_SYS_CALL 2

// Searches for hex pattern in file
void searchInFile(char *filename, Info info){
    FILE *fptr = NULL;

    fptr = openFile(filename, "rb");

    int elements_read;
    int flag = 0;

    unsigned long *offsets_list = NULL;
    unsigned long inserted_offsets = 0;

    uint8_t byte;
    long search_byte = 0;

    char byte_hex[3] = { info.search_value[2], info.search_value[3] };
    unsigned int hex_position = 2;
    long pointer;



    if(fptr == NULL){
        printf("\nERROR:'%s': CANNOT PROCCESS FILE\n-----\n", filename);
        return;
    }

    while(!feof(fptr)){
        pointer = ftell(fptr);
        elements_read = fread(&byte, sizeof(byte), 1, fptr);
 
        if(elements_read == 1){
            byte_hex[0] = info.search_value[hex_position]; 
            byte_hex[1] = info.search_value[hex_position + 1];
            search_byte = strtoul(byte_hex, NULL, 16);

            if(byte == search_byte){
                flag = 1;
                hex_position += 2;
                if(hex_position == strlen(info.search_value)){
                    if(flag == 1){
                        offsets_list = realloc(offsets_list, sizeof(unsigned long) * (inserted_offsets + 1));
                        if(offsets_list == NULL){
                            ERROR(ERR_SYS_CALL, "Cannot reallocate memory for offsets list");
                        }
                        offsets_list[inserted_offsets] = pointer;
                        inserted_offsets++;
                    }
                    hex_position = 2;
                }
            } else {
                hex_position = 2;
                flag = 0;
            }
        }
 
    }

    if(offsets_list != NULL){
        if(info.output){
            printSearchInFile(filename, info, offsets_list, inserted_offsets);
        } else {
            printSearch(filename, info, offsets_list, inserted_offsets);
        }
    }

    free(offsets_list);
    fclose(fptr);
}

// Reads file and outputs data
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
    rewind(fptr);

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

// Manages the reading of bytes
void readBytes(FILE *fptr, char *filename, Info info){
        uint8_t byte;
        int elements_read;

        HashTable *hashTable = NULL;

        unsigned int *combinations = NULL;
        unsigned int inserted_combinations = 0;
        int byte_position = 0;

        hashTable = createHashTable(BYTE_HASH_TABLE_SIZE, BYTE_HASH_VALUE);
        if (hashTable == NULL) {
            ERROR(ERR_SYS_CALL, "ERROR: Cannot allocate memory for hashtable");
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

// Manages the reading of duo bytes
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

// Manages the reading of quad bytes
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

// Prints data according to the parameters
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

// Verifies if string is a valid hexadecimal byte value
// Returns 0 for valis hex string, 1 if does not fulfill requirements
int validateHexString(char *string){
    // Strings must inicialize with '0x'
    if(string[0] != '0' || string[1] != 'x'){
        printf("\nfreqCounter:invalid value ‘%s’ for -s/--search (needs to be an integer value of bytes)\n", string);
        return 1;
    }

    // Strings must represent values between 1 byte to 32 bytes
    if(((strlen(string) - 2) % 2 )!= 0 || (strlen(string) - 2) == 0 || (strlen(string) - 2) > 64){
        printf("\nfreqCounter: invalis value '%s' for -s/--search (needs to be specified in HEX format)\n", string);
        return 1;
    }

    return 0;
}