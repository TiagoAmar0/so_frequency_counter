#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>


#include "table.h"
#include "memory.h"

void printHashtableCompact(char* filename, HashTable *hashTable, long filesize, unsigned int *combinations, unsigned int inserted_combinations){
        
        List *list;
        int hashCode;

        printf("freqCounter:'%s':%ld bytes:", filename, filesize);

        for(unsigned int i = 0; i < inserted_combinations; i++){
            hashCode = hash(combinations[i], hashTable->size, hashTable->hash);

            for(list = hashTable->nodeList[hashCode]; list != NULL; list = list->next){
                if(list->key == combinations[i]){
                    printf("%lu", list->frequency);
                    continue;
                }
            }

        }

        printf(":%lu\n", hashTable->totalRead);
        printf("------------------------------------------------------------\n\n");
}

void printHashtable(char* filename, HashTable *hashTable, long filesize, unsigned int *combinations, unsigned int inserted_combinations, char *description) {
        
        List *list;
        int hashCode;

        printf("freqCounter:'%s': %lu bytes\n", filename, filesize);

        for(unsigned int i = 0; i < inserted_combinations; i++){
            hashCode = hash(combinations[i], hashTable->size, hashTable->hash);

            for(list = hashTable->nodeList[hashCode]; list != NULL; list = list->next){
                if(list->key == combinations[i]){
                    printf("%s %u: %lu\n", description, list->key, list->frequency);
                    continue;
                }
            }

        }

        printf("sum: %lu\n", hashTable->totalRead);
        printf("----------\n\n");
}

HashTable *createHashTable(unsigned int size, int hash){

    HashTable *hashTable = NULL;

    if(size < 1){
        return NULL;
    }

    hashTable = (HashTable *) MALLOC(sizeof(HashTable));
    if(hashTable == NULL){
        printf("ERROR: Cannot allocate memory for hashtable\n");
        exit(1);
    }

    hashTable->nodeList = (List **) MALLOC(size * sizeof(List));
    if(hashTable->nodeList == NULL){
        printf("ERROR: Cannot allocate memory for hashtable\n");
        exit(1);
    }

    memset(hashTable->nodeList, 0, size * sizeof(List));

    hashTable->size = size;
    hashTable->hash = hash;
    hashTable->totalRead = 0;

    return hashTable;
}

// Releases every node memory
void freeHashTable(HashTable *hashTable)
{
        List *tmp;
        unsigned int i;

        if (hashTable == NULL) {
                return;
        }

        for (i = 0; i < hashTable->size; ++i) {
                if (hashTable->nodeList[i] != NULL) {
                        /* Traverse the list and free the nodes. */
                        while(hashTable->nodeList[i] != NULL) {
                          tmp = hashTable->nodeList[i]->next;
                          free(hashTable->nodeList[i]);
                          hashTable->nodeList[i] = tmp;
                        }
                        free(hashTable->nodeList[i]);
                }
        }
        free(hashTable->nodeList);
        free(hashTable);
}

// Hash function that will return the index of the hash table
// Adapted from: http://www.cse.yorku.ca/~oz/hash.html 
int hash(unsigned int byte, int size, int hash){

    char *buffer;
 
    buffer = itoa(byte, 10);
    int c;

    while ((c = *buffer++))
        hash = ((hash << 5) + hash) + c;

    return byte % size;
}

// Function that converts a number to a string
// Adapted from http://www.strudel.org.uk/itoa/
char* itoa(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}

// Validates if the node is an insetion or if it was already inserted and is just needed to update values
unsigned int *nodeHandler(HashTable *hashTable, List *node, unsigned int *combinations, unsigned int *inserted_combinations){
    unsigned int hashCode = hash(node->key, hashTable->size, hashTable->hash);
    //printf("BYTE -> %u | SIZE -> %u | HASH -> %i\n", node->key, hashTable->size, hashTable->hash);
    List *temp = hashTable->nodeList[hashCode];

    if(hashTable->nodeList[hashCode]){
        temp = hashTable->nodeList[hashCode];
        while(temp != NULL){
            // If already exists a node containing the byte we are inserting, just increment the frequency
            if(temp->key == node->key){
                temp->frequency++;
                break;
            }

            // Check next linked node
            temp = temp->next;
        }

        if(temp == NULL){
            node->next = hashTable->nodeList[hashCode];
            node->frequency = 1;
            hashTable->nodeList[hashCode] = node;
            
            // Add new byte(s) combination to dynamic vector
            combinations = realloc(combinations, sizeof(unsigned int) * (*inserted_combinations + 1));
            if(combinations == NULL){
                printf("Error reallocating memory for a new combination. Aborting\n\n");
                exit(1);
            }

            combinations[*inserted_combinations] = node->key;
            (*inserted_combinations)++;

        } else {
            free(node);
        }
    } else {
        node->next = NULL;
        node->frequency = 1;
        hashTable->nodeList[hashCode] = node;
        // Add new byte(s) combination to dynamic vector
        combinations = realloc(combinations, sizeof(unsigned int) * (*inserted_combinations + 1));
        if(combinations == NULL){
            printf("Error reallocating memory for a new combination. Aborting\n\n");
            exit(1);
        }

        combinations[*inserted_combinations] = node->key;
        (*inserted_combinations)++;
    }

    hashTable->totalRead++;
    return combinations;
}

// Inserts / Updates information about a byte combination on the HashTable
unsigned int *insertInHashTable(HashTable *hashTable, unsigned int byte, unsigned int *combinations, unsigned int *inserted_combinations){
    List *node;

    if(hashTable == NULL){
        return combinations;
    }

    node = MALLOC(sizeof(List));
    if(node == NULL){
        return combinations;
    }

    node->key = byte;

    combinations = nodeHandler(hashTable, node, combinations, inserted_combinations);

    return combinations;
}