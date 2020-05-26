#ifndef COUNTER_H
#define COUNTER_H

#define BYTE_HASH_TABLE_SIZE 255
#define DUO_BYTE_HASH_TABLE_SIZE 65535
#define QUAD_BYTE_HASH_TABLE_SIZE 500000

#define BYTE_HASH_VALUE 179
#define DUO_BYTE_HASH_VALUE 37987
#define QUAD_BYTE_HASH_VALUE 52057

#include "table.h"

FILE *openFile(char *filename, char* mode);
void readFile(char *filename, int mode);
void NodeHandler(HashTable *hashTable, List *node);
void readBytes(FILE *fptr, char *filename );
void readDuoBytes(FILE *fptr, char *filename);
void readQuadBytes(FILE *fptr, char *filename);
int compareNumbers(const void * a, const void * b);
#endif