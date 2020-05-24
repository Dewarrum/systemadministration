#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void int_to_bin_digit(unsigned int in, int* out) {
	unsigned int mask = 1U << (32 - 1);
	int i;
	for (i = 0; i < 32; i++) {
		out[i] = (in & mask) ? 1 : 0;
		in <<= 1;
	}

	printf("Converted %d to binary\n", in);
}

int fd[2];
int child_counter = 0;
int child_result = 0;

void child_handler(int nsig) {
	if (child_counter == 32) {
		close(fd[1]);
		close(fd[0]);

		printf("Child received %d\n", child_result);
		exit(0);
	}

	int bit;
	size_t size;
	pid_t ppid = getppid();
	size = read(fd[0], (void*)&bit, sizeof(int));
	printf("Child received %d-th bit\n", child_counter);

	if (size < sizeof(int)) {
		printf("Can\'t read string\n");
		exit(-1);
	}

	child_result = child_result | 1 << child_counter;
	child_counter++;

	kill(ppid, SIGUSR2);

	exit(0);
}

void child_handler_1(int nsig) {
	pid_t ppid = getppid();
	kill(ppid, SIGUSR2);
	printf("Start processing\n");
}

int parent_counter = 0;
int bin_array[32];
int result;

void parent_handler(int nsig) {
	if (parent_counter == 32) {
		close(fd[0]);
		close(fd[1]);
		exit(0);
	}

	size_t size;
	size = write(fd[0], (void*)&bin_array[parent_counter], sizeof(int));
	printf("Parent sent %d-th bit\n", parent_counter);

	if (size < sizeof(int)) {
		printf("Can\'t read string\n");
		exit(-1);
	}

	parent_counter++;
	kill(result, SIGUSR2);
}

int main(int argc, char **argv) {
	size_t size;
	int number = atoi(argv[1]);

	if (pipe(fd) < 0) {
		printf("Can\'t create pipe\n");
		exit(-1);
	}

	result = fork();

	if (result < 0) {
		printf("Can\'t fork child\n");
		exit(-1);
	} else if (result > 0) {
		(void) signal(SIGUSR2, parent_handler);
		int_to_bin_digit(number, bin_array);

		kill(result, SIGUSR1);

		while (1);
	} else {
		(void) signal(SIGUSR2, child_handler);
		(void) signal(SIGUSR1, child_handler_1);

		while (1);
	}

	return 0;
}
