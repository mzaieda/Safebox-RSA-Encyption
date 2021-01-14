#include <CommandLineArguments.h>
#include <Lottery.h>


int main(int argc, char **argv)
{    

    while(true){
		char ** arg;//declaring a pointer to a pointer of type char which creates a mtrix
		char command [1024];  //declaing an array of characters of 1GB of memory
		memset (command,0,1024); //setting all the elements to zero in the array

		printf ("$SB > ");   //printing the terminal pop uo for the user to enter a command
		fgets (command,1000,stdin);   //getting an input from the user and storing it to command with sapce up to 1000 bytes
				if (command[strlen(command)-1] == '\n')   //getting the string length of the command 
					command[strlen(command)-1] = '\0';  //assiging to the command of element that has the strnglength to null character

		if (strcasecmp(command, "exit") == 0) {  //if the command is equivelant to exit then break the loop
			break;

		}
		
		
		else {
			pid_t pid = fork();  //forking the process
			if (pid == 0){ //if child process do the commands below

				time_t start = time(NULL);
                CommandLineArguments cmlarg;
                if (cmlarg.parser(argc,argv))
                {
                    if (!cmlarg.execute())
                    {
                        if ( strcmp(cmlarg.getErrorString(),"") != 0 )
                            printf ("%s\n",cmlarg.getErrorString());
                    }        
                }
                else
                {
                    if ( strcmp(cmlarg.getErrorString(),"") != 0 )
                        printf ("%s",cmlarg.getErrorString());
                    printf ("usage: %s --command <import/export> --input-file <file-name> --output-file <file-name> --working_dir <working directory full path> --key-file <Full path to file containing the AES key> --block-size <size of a block> --shreds <# of shreds>\n",argv[0]);
                    exit(1);
            }
                time_t end = time(NULL);
                printf ("Total time: %lu\n",end-start);
	
			free (arg);   //freeing the arg from heap
			} else { //parent processor
				waitpid(pid, NULL, 0);   //wait for th pid until reaches 0;

					
			}
		}
	}		
    
    
 
    return 0;
}