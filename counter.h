#ifndef COUNTER_H
#define COUNTER_H

#define BYTE_HASH_TABLE_SIZE 255
#define DUO_BYTE_HASH_TABLE_SIZE 65535
#define QUAD_BYTE_HASH_TABLE_SIZE 500000

#define BYTE_HASH_VALUE 179
#define DUO_BYTE_HASH_VALUE 37987
#define QUAD_BYTE_HASH_VALUE 52057

#define NOT_FOUND -1

#include "table.h"

typedef struct Info {
    int compact;
    long file_size;
    int discrete;
    int mode;
    int search;
    int output;
    long *discrete_values;
    int discrete_values_total;
    char *search_value;
    char *output_target;
} Info;

void readFile(char *filename, Info info);
void NodeHandler(HashTable *hashTable, List *node);
void readBytes(FILE *fptr, char *filename, Info info);
void readDuoBytes(FILE *fptr, char *filename, Info info);
void readQuadBytes(FILE *fptr, char *filename, Info info);
int compareNumbers(const void * a, const void * b);
unsigned int findInLongArray(unsigned int value, long *array, unsigned int total);
void outputData(char *filename, HashTable *hashTable, Info info, unsigned int *combinations, unsigned int inserted_combinations, char* description);
int validateHexString(char *string);
void searchInFile(char *filename, Info info);

#endif