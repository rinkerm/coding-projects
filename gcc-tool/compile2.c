#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <libgen.h>

static int MAX = 100;

/*
Compile.c
Tool that works with GCC and AR to compile complex C libraries
*/

void *compile(void *args)
{
/*
compile
This is the thread routine which is called to handle all system calls made by threads.
It takes in a string which is used to execute a program in the linux shell
It uses popen so it can dump all logs from GCC or AR to the console so the user can note errors
*/

	FILE *cmd=popen(args,"r"); //This is the statement that calls the linux shell. I used popen so I can access what it would write to the console
    char result[24]={0x0}; //The following lines receive the data from stdin and prints it out in the console for the user to see
    while (fgets(result, sizeof(result), cmd) !=NULL) {printf("%s\n", result);}
    pclose(cmd); //We then close the cmdline interface to avoid memory leaks
}

void compiler(int argc, char** args)
{
/*
compiler
This is the helper function which creates a thread and handles its execution.
Waiting for a thread to execute is hard coded into this function.
It is less efficient but it ensures that there are no race conditions which could break the tool.
It also handles error reporting and will exit the program if there is a critical error which causes us to be unable to generate a new thread
*/
	pthread_t threads[argc];
	for(int i = 0;i<argc;i++)
	{
		int error; //We need to declare an integer so we can record the error code that comes back from creating a thread (if it works then it will return 0)
		error = pthread_create(&threads[i], NULL, &compile, args[i]); //We then set error to the return value from creating our thread.
		if(error) //Checking if error is nonzero and then reporting the error code
		{
			printf("Error Creating Thread: %d\n",error);
			exit(0);
		}
	}
	for(int j=0;j<argc;j++)
	{

	pthread_join(threads[j], NULL);

	}
	return;
}
void traverse_dir(int n, char* path)
{
/*
traverse_dir
This is the meat of the tool
It is a recursive function which traverses a directory and compiles all files and libraries within it
Essentially, It opens the given directory and looks through. It records what it finds and recursively calls itself on every directory
It then compiles all .c files into .o with gcc
If it is not on the first level of execution it takes all the .o and .a files (if it has subdirectories) and packages them into a .a file with ar which is then saved to the directory one step above it.
If it is on the first level of execution it instead packages all .o and .a files into a single executable named after the current working directory
*/
	char** files = malloc(MAX * sizeof(char *)); //Here we allocate memory for the pointers in the string arrays we will use to record filenames
	int fileno = 0; //We also initialize a variable to let us know how many .c files we have
	char files2[10000];
	DIR* dir; //We now setup our directory traversal tools
	struct dirent *ent;
	struct stat states;
	dir = opendir(path); //Open the directory
	
	while((ent=readdir(dir))!=NULL) //We can then traverse through the directory
	{
		stat(ent->d_name,&states); //This gives us the next item in the directory
		if(!strcmp(".", ent->d_name) || !strcmp("..",ent->d_name) || !strcmp("compile",ent->d_name) || !strcmp("a.out",ent->d_name) || !strcmp(strrchr(ent->d_name, '\0') - 2, ".a")) {continue;} //Since in directory structures there will always be a . and .. directory we must ignore these to avoid an infinite loop. We also want to avoid messing with our executable AND we want to make sure we don't touch any already generated .a files
		else
		{
			//Here we allocate memory for our filename strings and ensure that the data within is clear (This is to avoid bugs where shorter filenames end up with parts of longer filenames in them).
			char filename[500]; 
			char filename2[500];
			char newpath[500];
			memset(filename, '\0', sizeof(filename));
			memset(filename2, '\0', sizeof(filename));
			memset(newpath, '\0', sizeof(newpath));
			
			strcpy(newpath,path); //Now we create a string containing the path to each new item in the directory we are looking at. This is to tell gcc and ar exactly where to look
			strcat(newpath, "/");
			strcat(newpath,ent->d_name);
			strcat(newpath,"\0");
			
			if(ent->d_type==4) //This is a check to see if we are looking at a directory
			{
				
				strcpy(filename2,newpath); //We get the path to the directory and generate the name with .a appended for use later
				strcat(filename2,".a ");	//We then add this to a string which we will use in our ar call later
				strcat(files2,filename2);
				traverse_dir(n+1,newpath); //Recursive call to handle the contents of the directory
			}
			else if(!strcmp(strrchr(ent->d_name, '\0') - 2, ".c"))
			{{
				strcpy(filename,newpath); //Get the filename to use for later
				files[fileno] = (char *)malloc(strlen(filename)+1);
				strcpy(files[fileno],filename);
				strncpy(filename2,newpath,strlen(path)+strlen(ent->d_name));
				strcat(filename2,"o "); //Generate the new filename (.o) for later
				strcat(files2,filename2);
				fileno++;
			}}
		}
	}
	closedir(dir); //Close the directory to clean up memory
	
	char** calls = malloc(fileno * sizeof(char *));
	for(int i=0;i<fileno;i++) //Here we loop through all files found and prepare them for compilation. We free the memory as we go to prevent using too much
	{
		char args[10000];
		char destfile[500];
		memset(args, '\0', sizeof(args));
		memset(destfile, '\0', sizeof(destfile));
		strncpy(destfile,files[i],strlen(files[i])-1);
		strcat(destfile,"o");
		sprintf(args,"gcc -c %s -o %s",files[i],destfile);
		calls[i] = (char*)malloc(strlen(args)+1);
		strcpy(calls[i],args);
		free(files[i]);
	}
	free(files); //Delete the array at the end.
	compiler(fileno,calls); //Here we send off a batch of files to our helper function to create threads to compile all our files
	for(int j =0;j<fileno;j++) //We do this to free up memory
	{
		free(calls[j]);
	}
	free(calls);
	
	char dest1[500]; //Here we declare some strings we are going to use to generate our system calls
	char** args2 = malloc(sizeof(char*));;
	char cwd[500];
	memset(dest1, '\0',sizeof(dest1)); //Clear the memory allocated incase there are some stray values
	memset(cwd, '\0',sizeof(cwd));
	
	if(n>0) //IF we are not in the first tier of execution
	{
		args2[0] = (char*) malloc(10000);
		strcpy(dest1,path); //Add a .a to the end of our destination as we are making a library
		strcat(dest1,".a ");
		
		strcpy(args2[0],"ar rcs "); //This is the beginning of our ar call
		strcat(args2[0], dest1);
		strcat(args2[0],files2);
		strcat(args2[0],"\0"); //Add a nullbyte and call the helper function to create a thread for our systemcall
		compiler(1,args2);
	}
	else //IF we are in the first tier of execution we are making an executable
	{
		
		char cwd[1000]; //We need to get the base name of our current working directory as this will be the name of our executable
		getcwd(cwd, sizeof(cwd));
		strcpy(dest1,basename(cwd));
		strcpy(args2[0],"gcc -o "); //We can now start to lay out our gcc system call
		strcat(args2[0],dest1); //Add our destination
		strcat(args2[0]," ");
		strcat(args2[0],files2);
		strcat(args2[0],"\0"); //Add a nullbyte and compule
		compiler(1,args2);
		
	}
}

int main(int argc, char* argv[])
{
/*
main
This is the main function
It calls traverse_dir
*/
	traverse_dir(0,".");
	return 0;
}
