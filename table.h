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
unsigned int *nodeHandler(HashTable *hashTable, List *node, unsigned int *combinations, unsigned int *inserted_combinations);
void freeHashTable(HashTable *hashTable);
HashTable *createHashTable(unsigned int size, int hash);
unsigned int *insertInHashTable(HashTable *hashTable, unsigned int byte, unsigned int *combinations, unsigned int *inserted_combinations);
void printHashtable(char* filename, HashTable *hashTable, long filesize, unsigned int *combinations, unsigned int inserted_combinations, char *description);
void printHashtableCompact(char* filename, HashTable *hashTable, long filesize, unsigned int *combinations, unsigned int inserted_combinations);

#endif 