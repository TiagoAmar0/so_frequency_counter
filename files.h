#ifndef FILES_H
#define FILES_H

#include <dirent.h>

DIR *openDirectory(char *path);
char* checkIfIsRegularFile(char *folder, char *name, int *status);

#endif