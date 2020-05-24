#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void my_handler(int nsig) {
	int status;
	pid_t pid;

	if ((pid = waitpid(-1, &status, 0)) < 0) {
		printf("Some error on waitpid errno = %d\n", errno);

	} else {
		if ((status & 0xff) == 0) {
			printf("Process %d was exited with status %d\n", pid, status >> 8);

		} else if ((status & 0xff00) == 0) {
			printf("Process %d killed by signal %d %s\n", pid, status & 0x7f,
			       (status & 0x80) ? "with core file" : "without core file");
		}
	}
}

int main(void) {
	pid_t pid;
	int i;

	(void) signal(SIGCHLD, my_handler);

	for (i = 0; i < 5; i++) {
		if ((pid = fork()) < 0) {
			printf("Can\'t fork child %d\n", i);
			exit(1);
		} else if (pid == 0) {
			exit(200 + i);
		}
	}

	while (1);
	return 0;

}
