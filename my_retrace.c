#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

int main( int argc, char **argv)
{ 
    FILE * fp_tracefile = fopen(argv[1],"r");
    char line_buf[1024];
    int ret;
    char* ptr;
    
    gScriptEntry[i].thread_num = atoi(ptr);

    ptr = strtok( NULL, " ");
    gScriptEntry[i].time = atoll(ptr);
    
    ptr = strtok( NULL, " ");
    gScriptEntry[i].cmd = (char*)malloc(strlen(ptr)+1);
    strcpy(gScriptEntry[i].cmd, ptr);
            
    while( ptr = strtok( NULL, " "))
    {
        gScriptEntry[i].args[args_num] = (char*)malloc(strlen(ptr)+1);
        strcpy(gScriptEntry[i].args[args_num], ptr);
        args_num++;
        if(args_num == 3) break;
    }
    gScriptEntry[i].arg_num = args_num;
    /*
	* Scan script file and count lines
	*/
	do {
        memset(line_buf, 0, sizeof(line_buf));
		ret = readline(fp_tracefile, line_buf, sizeof(line_buf));
		if(ret > 0) {
            ptr = strtok(line_buf, " ");
			line_count++;		
		}
	}while(ret > 0);
	printf("line count : %d\n", line_count);
    return 0;
}