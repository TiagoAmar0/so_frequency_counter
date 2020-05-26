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
#include <dirent.h>

#include "debug.h"
#include "memory.h"
#include "counter.h"
#include "args.h"
#include "table.h"
#include "files.h"

struct gengetopt_args_info args_info;

#define ERR_ARGS 1
#define ERR_SYS_CALL 2

int main(int argc, char *argv[]){

	if (cmdline_parser(argc,argv,&args_info) !=	0){
		exit(1);
	}

	char *path = NULL;
	
	if(!args_info.file_given && !args_info.dir_given){
		ERROR(ERR_ARGS, "--file/-f or --dir/-d parameters must be entered.");
	}

	if((args_info.discrete_given && args_info.compact_flag) || (args_info.compact_flag && args_info.search_given)){
		ERROR(ERR_ARGS, "--compact/-c is not compatible with --search/-s or --discrete.");
	}

	if((args_info.search_given && args_info.discrete_given)){
		ERROR(ERR_ARGS, "--search/-s is not compatible with --discrete.");
	}


	// Manages the input of --file / -f option
	if(args_info.file_given){
		for(unsigned int i = 0; i < args_info.file_given; i++){
			readFile(args_info.file_arg[i], args_info.mode_arg);
		}
	}

	// Manages the input of --dir / -d option
	if(args_info.dir_given){

		// Tries to open a directory
		DIR *directory = openDirectory(args_info.dir_arg);

		struct dirent *dirent;
		int status;
		
		while((dirent = readdir(directory)) != NULL){
			path = checkIfIsRegularFile(args_info.dir_arg, dirent->d_name, &status);
			if(status == 0){
				readFile(path, args_info.mode_arg);
			}
		}

		closedir(directory);
	}
	free(path);
	return 0;
}

