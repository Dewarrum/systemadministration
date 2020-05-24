#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void my_handler(int nsig) {
	printf("Receive signal %d, CTRL-C pressed\n", nsig);
	exit(0);
}

void another_handler(int nsig) {
	printf("Receive signal %d, CTRL-4 pressed\n", nsig);
}

int main(void) {

	pid_t pid;
	int i;

	(void) signal(SIGINT, my_handler);
	(void) signal(SIGQUIT, another_handler);

	while (1);
	return 0;
}
