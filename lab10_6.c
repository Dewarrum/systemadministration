#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int fd[2];

void child_handler(int nsig) {
	char resstring[14];
	size_t size;
	pid_t ppid = getppid();
	size = read(fd[0], resstring, 14);

	if (size < 0) {
		printf("Can\'t read string\n");
		exit(-1);
	}

	printf("%s\n", resstring);

	size = write(fd[1], "Response from child", 20);

	if (size != 20) {
		printf("Can\'t write all string\n");
		exit(-1);
	}

	close(fd[1]);
	close(fd[0]);

	kill(ppid, SIGUSR2);

	printf("Child sends message to parent with %d pid\n", ppid);

	exit(0);
}

void parent_handler(int nsig) {
	size_t size;
	char resstring[20];
	size = read(fd[0], resstring, 14);

	if (size < 20) {
		printf("Can\'t read string\n");
		exit(-1);
	}

	printf("%s\n", resstring);


	close(fd[0]);
	close(fd[1]);

	exit(0);
}

int main() {
	int result;
	size_t size;
	char resstring[14];

	if (pipe(fd) < 0) {
		printf("Can\'t create pipe\n");
		exit(-1);
	}

	result = fork();

	if (result < 0) {
		printf("Can\'t fork child\n");
		exit(-1);

	} else if (result > 0) {
		printf("Parent process start, pid: %d\n", getpid());

		(void) signal(SIGUSR2, parent_handler);
		size = write(fd[1], "Hello, world!", 14);

		if (size != 14) {
			printf("Can\'t write all string\n");
			exit(-1);
		}

		printf("Before kill\n");

		kill(0, SIGUSR1);

		printf("After kill\n");

		while (1);
	} else {
		printf("Child process start, pid: %d\n", getpid());
		(void) signal(SIGUSR1, child_handler);

		while (1);
	}

	return 0;
}