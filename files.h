#ifndef FILES_H
#define FILES_H

#include <dirent.h>
#include "table.h"

DIR *openDirectory(char *path);
char* checkIfIsRegularFile(char *folder, char *name, int *status);
FILE *openFile(char *filename, char* mode);
void printInFile(char* filename, HashTable *hashTable, char* target_file, long filesize, unsigned int *combinations, unsigned int inserted_combinations, char *description);
void printInFileCompact(char *filename, HashTable *hashTable, char* target_file, long filesize, unsigned int *combinations, unsigned int inserted_combinations);

#endif