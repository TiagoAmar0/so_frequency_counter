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

	// Args validations
	if (cmdline_parser(argc,argv,&args_info) !=	0){
		exit(1);
	}

	// Variables to measure execution time
	clock_t start, end;

	if(args_info.time_given){
		start = clock();
	}
	
	if(!args_info.file_given && !args_info.dir_given){
		ERROR(ERR_ARGS, "--file/-f or --dir/-d parameters must be entered.");
	}

	if((args_info.discrete_given && args_info.compact_flag) || (args_info.compact_flag && args_info.search_given)){
		ERROR(ERR_ARGS, "--compact/-c is not compatible with --search/-s or --discrete.");
	}

	if((args_info.search_given && args_info.discrete_given)){
		ERROR(ERR_ARGS, "--search/-s is not compatible with --discrete.");
	}

	// If search mode is active, validates imput
	if(args_info.search_given){
		// Validates if string is a valid hexadecimal value
		if(validateHexString(args_info.search_arg) == 1){
			ERROR(ERR_ARGS, "freqCounter:invalid value ‘%s’ for -s/--search (needs to be specified in HEX format)", args_info.search_arg);
		}
	}

	// Args validation end

	char *path = NULL;
	Info info;

	// Define info according to parameters
	if(args_info.compact_flag){
		info.compact = 1;
	} else {
		info.compact = 0;
	}
	
	if(args_info.discrete_given){
		info.discrete = 1;
		info.discrete_values = args_info.discrete_arg;
		info.discrete_values_total = args_info.discrete_given;
	} else {
		info.discrete = 0;
	}

	info.mode = args_info.mode_arg;

	if(args_info.search_given){
		info.search = 1;
		info.search_value = args_info.search_arg;
	} else {
		info.search = 0;
	}

	if(args_info.output_given){
		info.output = 1;
		info.output_target = args_info.output_arg;
		// Creates / Truncates file
		FILE *fptr = NULL;
		fptr = openFile(info.output_target, "w");
		if(fptr == NULL){
			WARNING("\nERROR:'%s': CANNOT PROCCESS DATA TO OUTPUT FILE\n-----\n", info.output_target); 
			exit(1);
		}
		fclose(fptr);
	} else {
		info.output = 0;
	}
	// END: Define info according to parameters


	// Manages the input of --file / -f option
	if(args_info.file_given){
		for(unsigned int i = 0; i < args_info.file_given; i++){
			if(args_info.search_given){
				searchInFile(args_info.file_arg[i], info);
			} else {
				readFile(args_info.file_arg[i], info);
			}
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
				if(args_info.search_given){
					searchInFile(path, info);
				} else {
					readFile(path, info);
				}
			}
		}

		closedir(directory);
	}

	// Free resources
	free(path);


	if(args_info.time_given){
		end = clock();

		printf("\nTime: %f", ((double) (end - start)) / CLOCKS_PER_SEC);
	}
	
	cmdline_parser_free (&args_info);
	return 0;
}

