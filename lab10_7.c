#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void int_to_bin_digit(unsigned int in, int* out) {
	int initial = in;
	unsigned int mask = 1U << (32 - 1);
	int i;
	for (i = 0; i < 32; i++) {
		out[i] = (in & mask) ? 1 : 0;
		in <<= 1;
	}

	printf("Converted %d to binary: ", initial);

	for (i = 0; i < 32; i++) {
		printf("%d ", out[i]);
	}

	printf("\n");
}

int fd[2];
int child_counter = 0;
int child_result = 0;

void child_handler(int nsig) {
	if (child_counter == 31) {
		close(fd[1]);
		close(fd[0]);

		printf("Child received %d\n", child_result);
		exit(0);
	}

	char bit[2];
	size_t size;
	pid_t ppid = getppid();
	size = read(fd[1], bit, 2);
	printf("Child received bit[%d] = %d\n", child_counter, bit[0]);

	if (size < 2) {
		printf("Can\'t read string\n");
		exit(-1);
	}

	child_result = child_result | bit[0] << child_counter;
	child_counter++;

	kill(ppid, SIGUSR1);
	printf("Child sends signal to parent with id %d\n", ppid);
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
	char bit[2];
	bit[0] = (char) bin_array[parent_counter];
	bit[1] = '\0';
	size = write(fd[0], bit, 2);
	printf("Parent sent bit[%d] = %d\n", parent_counter, bin_array[parent_counter]);

	if (size < 2) {
		printf("Can\'t read string\n");
		exit(-1);
	}

	parent_counter++;
	printf("Parent sent to child process with %d id\n", result);
	kill(result, SIGUSR2);
}

int main(int argc, char **argv) {
	size_t size;
	printf("Got %s as parameter\n", argv[1]);
	int number = atoi(argv[1]);
	printf("Parsed %s as %d\n", argv[1], number);

	if (pipe(fd) < 0) {
		printf("Can\'t create pipe\n");
		exit(-1);
	}

	result = fork();

	if (result < 0) {
		printf("Can\'t fork child\n");
		exit(-1);
	} else if (result > 0) {
		(void) signal(SIGUSR1, parent_handler);
		int_to_bin_digit(number, bin_array);

		printf("Parent sent SIGUSR1 to child process with %d id\n", result);

		while (1);
	} else {
		printf("Child (%d id) subscribes on SIGUSR signals\n", getpid());
		(void) signal(SIGUSR2, child_handler);

		pid_t ppid = getppid();
		printf("Child sends signal to parent with %d pid\n", ppid);
		kill(ppid, SIGUSR1);
		printf("Start processing\n");

		while (1);
	}

	return 0;
}
