#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct List {
    unsigned int key;
    unsigned long frequency;
    struct List *next;
} List;

typedef struct HashTable
{
  unsigned int size;
  unsigned long totalRead;
  int hash;
  List **nodeList;
} HashTable;

int hash(unsigned int byte, int size, int hash);
char* itoa(int val, int base);
unsigned int *nodeHandler(HashTable *hashTable, List *node, unsigned int *combinations, unsigned int *insertedCombinations);
void freeHashTable(HashTable *hashTable);
HashTable *createHashTable(unsigned int size, int hash);
unsigned int *insertInHashTable(HashTable *hashTable, unsigned int byte, unsigned int *combinations, unsigned int *insertedCombinations);
int printHashtable(char* filename, HashTable *hashTable, unsigned int *combinations, unsigned int insertedCombinations, char *description);

#endif 