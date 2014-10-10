// Simple shell for Linux.

#include <stdio.h>
#define TRUE 1

int main()
{
	while (TRUE)
	{
		// print prompt and read into string input.
		print_prompt();
		char input[256];
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

void scan_input(char* input)
{
	scanf("%s", input);
}

// function to be called if this is parent process.
void process_parent(int* statloc)
{
	waitpid(-1, statloc, 0);
}

// function to be called if this is child process.
void process_child(const char* input)
{
	execve(input, NULL, NULL);
}
