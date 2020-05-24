#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void my_handler(int nsig) {
	printf("You hit %d\n", nsig);
	exit(0);
}

int main(void) {

	pid_t pid;
	int i;

	(void) signal(SIG_DFL, my_handler);

	while (1);
	return 0;
}
