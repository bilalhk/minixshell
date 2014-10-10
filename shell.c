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

		// perform a fork and process input.
		int pid = fork();
		if (pid != 0)
		{
			// code for parent process.
			int statloc;
			process_parent(&statloc);
		}
		else
		{
			// code for forked child process.
			process_child(input);
		}

	}
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
	for (int i = 0; token != NULL; i++)
	{
		input[i] = (char*) malloc(sizeof(char) * strlen(token));
		strcpy(input[i], token);
		token = strtok(NULL, " ");
	}
	input[i] = NULL;
}

// function to be called if this is parent process.
void process_parent(int* statloc)
{
	waitpid(-1, statloc, 0);
}

// function to be called if this is child process.
void process_child(const char** input)
{
	execve(input[0], input, NULL);
}
