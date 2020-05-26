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