#ifndef FILES_H
#define FILES_H

#include <dirent.h>
#include "table.h"
#include "counter.h"

DIR *openDirectory(char *path);
char* checkIfIsRegularFile(char *folder, char *name, int *status);
FILE *openFile(char *filename, char* mode);
void printInFile(char* filename, HashTable *hashTable, char* output_target, long filesize, unsigned int *combinations, unsigned int inserted_combinations, char *description);
void printInFileCompact(char *filename, HashTable *hashTable, char* output_target, long filesize, unsigned int *combinations, unsigned int inserted_combinations);
void printSearchInFile(char *filename, Info info, unsigned long *offsets_list, unsigned long inserted_offsets);
void printSearch(char *filename, Info info, unsigned long *offsets_list, unsigned long inserted_offsets);
#endif