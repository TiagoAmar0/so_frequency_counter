#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#include "debug.h"
#include "memory.h"
#include "counter.h"
#include "args.h"
#include "table.h"

struct gengetopt_args_info args_info;

int main(int argc, char *argv[]){

	if (cmdline_parser(argc,argv,&args_info) !=	0){
		exit(1);
	}
	
	if(!args_info.file_given && !args_info.dir_given){
		// ERROR	
	}

	if(args_info.file_given){
		for(unsigned int i = 0; i < args_info.file_given; i++){
			readFile(args_info.file_arg[i], args_info.mode_arg);
		}
	}

	return 0;
}

