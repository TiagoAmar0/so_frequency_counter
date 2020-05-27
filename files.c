#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>

#include "files.h"
#include "memory.h"
#include "debug.h"

#define ERR_ARGS 1
#define ERR_SYS_CALL 2


void printInFile(char *filename, HashTable *hashTable, char* target_file, long filesize, unsigned int *combinations, unsigned int inserted_combinations, char *description){
    FILE *fptr = NULL;
    List *list;
    int hashCode;

    // Creates / Erases previous data
    fptr = openFile(target_file, "a");
    if(fptr == NULL){
        printf("\nERROR:'%s': CANNOT PROCCESS DATA TO OUTPUT FILE\n-----\n", target_file);
        return;
    }

    fprintf(fptr, "freqCounter:'%s': %lu bytes\n", filename, filesize);

    for(unsigned int i = 0; i < inserted_combinations; i++){
        hashCode = hash(combinations[i], hashTable->size, hashTable->hash);

        for(list = hashTable->nodeList[hashCode]; list != NULL; list = list->next){
            if(list->key == combinations[i]){
                if((fprintf(fptr, "%s %u: %lu\n", description, list->key, list->frequency) < 0)){
                    printf("\nERROR:'%s': CANNOT PROCCESS DATA TO OUTPUT FILE\n-----\n", target_file);
                    return; 
                }
                continue;
            }
        }

    }

    fprintf(fptr, "sum: %lu\n", hashTable->totalRead);
    fprintf(fptr, "----------\n");

    printf("INFO: output written \"to\" %s\n", target_file);

    fclose(fptr);
}

void printInFileCompact(char *filename, HashTable *hashTable, char* target_file, long filesize, unsigned int *combinations, unsigned int inserted_combinations){
    FILE *fptr = NULL;
    List *list;
    int hashCode;

    // Creates / Erases previous data
    fptr = openFile(target_file, "a");
    if(fptr == NULL){
        printf("\nERROR:'%s': CANNOT PROCCESS DATA TO OUTPUT FILE\n-----\n", target_file);
        return;
    }

    fprintf(fptr, "freqCounter:'%s':%lu bytes:", filename, filesize);

    for(unsigned int i = 0; i < inserted_combinations; i++){
        hashCode = hash(combinations[i], hashTable->size, hashTable->hash);

        for(list = hashTable->nodeList[hashCode]; list != NULL; list = list->next){
            if(list->key == combinations[i]){
                if((fprintf(fptr, "%lu",list->frequency) < 0)){
                    printf("\nERROR:'%s': CANNOT PROCCESS DATA TO OUTPUT FILE\n-----\n", target_file);
                    return; 
                }
                continue;
            }
        }

    }

    fprintf(fptr, ":%lu\n------------------------------------------------------------\n", hashTable->totalRead);

    fclose(fptr);
}

DIR *openDirectory(char *path){
    DIR *directory = opendir(path);

    if(directory == NULL){
        WARNING("Error opening directory %s", path);
        exit(1);
    }

    return directory;
}

// Receives a folder name and a file name
// Returns 0 if it is a regular file, otherwise returns 1
char* checkIfIsRegularFile(char *folder, char *name, int *status){

    struct stat metadata;
    int stat_inf;

    char *path = MALLOC(strlen(folder) + strlen(name) + 2);
    if(path == NULL){
        ERROR(ERR_SYS_CALL, "Error allocating memory. Aborting.");
    }

    strcpy(path, folder);
    strcat(path, "/");
    strcat(path, name);

    stat_inf = stat(path, &metadata);
    if (stat_inf == -1) {
        ERROR(ERR_SYS_CALL ,"stat() failed for entry: %s (%s)\n", path, strerror(errno));
    }

    if(!S_ISREG(metadata.st_mode)){
        *status = 1;
    } else {
        *status = 0;
    }
    return path;
}

// Tries to open a file in a given mode and returns the file pointer
FILE *openFile(char *filename, char* mode){
    FILE *f;

    f = fopen(filename, mode);

    return f;
}