// Simple shell for Linux.

#include <stdio.h>
#include <string.h>
#define TRUE 1

int main()
{
	while (TRUE)
	{
		// print prompt and read into string array input.
		print_prompt();
		char* input[64];
		scan_input(input);
		int input_len = array_len(input);
		int option = 0;

		// if last parameter is an & then run child process in background.
		/*if (input_len != 0 && !strcmp(input[input_len], "&"))
		{
			option = 1;
			input[input_len] = NULL;
		}*/

		/* pass to change_directory if input is a cd command.
		   Else perform a fork and process input. */
		if (input_len != 0 && !strcmp(input[0], "cd"))
		{
			change_directory(input);
		}
		else if (input_len != 0 && fork() != 0)
		{
			// code for parent process.
			int statloc;
			process_parent(&statloc, option);
		}
		else if (input_len != 0)
		{
			// code for forked child process.
			process_child(input);
		}

	}
}

// function that calculates length of array till NULL is encountered.
int array_len(const char** input)
{
	int count = 0;
	for (int i = 0; input[i] != NULL; i++)
	{
		count++;
	}
	return count;
}

// function for printing the prompt on the console.
void print_prompt()
{
	char dir[256];
	getcwd(dir, 256);
	printf("%s>> ", dir);
}

// read input from the terminal, tokenize it, allocate memory for each token on heap,
// and store pointers to tokens in char*[] input.
void scan_input(char** input)
{
	char text[256];
	fgets(text, 256, stdin);
	text[strlen(text) - 1] = '\0';
	char* token = strtok(text, " ");
	int i = 0;
	for (; token != NULL; i++)
	{
		input[i] = (char*) malloc(sizeof(char) * strlen(token));
		strcpy(input[i], token);
		token = strtok(NULL, " ");
	}
	input[i] = NULL;
}

// function to be called if this is parent process.
void process_parent(int* statloc, int option)
{
	if (!option)
	{
		waitpid(-1, statloc, 0);
	}
}

// function to be called if this is child process.
void process_child(const char** input)
{
	// try in working directory
	execve(input[0], input, NULL);
	
	char path[strlen(input[0]) + 12];
	
	// try in /bin
	strcpy(path, "/bin/"); 
	strcat(path, input[0]);
	execve(path, input, NULL);

	// try in /usr/bin
	strcpy(path, "/usr/bin/");
	strcat(path, input[0]);
	execve(path, input, NULL);
}

// function for changing the working directory to the path specified by the cd command.
void change_directory(const char** input)
{
	char cwd[128];
	getcwd(cwd, sizeof(char) * 128);

	/* if the cd command is .. then go up a level. If relative path provided go down.
	   Else change to absolute path provided. */
	if (!strcmp(input[1], ".."))
	{
		char* path_end = strrchr(cwd, '/');
		*path_end = '\0';
		chdir(cwd);
	}
	else if (input[1][0] != '/')
	{
		strcat(cwd, "/");
		strcat(cwd, input[1]);
		chdir(cwd);
	}
	else
	{
		chdir(input[1]);
	}
}
