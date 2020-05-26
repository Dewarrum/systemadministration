#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

int exited_processes_count = 0;

void *thread_func(void *arg) {
	int status;
	pid_t* tmp = (pid_t*) arg;
	pid_t pid = *tmp;

	// printf("Thread waiting for process %d started\n", pid);

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

	// printf("Thread waiting for process %d finished\n", pid);
	exited_processes_count++;

	return NULL;
}

void my_handler(int nsig) {
	/* pthread_t tid;
	(void) signal(SIGCHLD, my_handler);
	if (pthread_create(&tid, NULL, &thread_func, NULL)) {
		printf("Error creating new thread\n");
		exit(-1);
	}

	printf("Thread %d started\n", tid);
	pthread_join(tid, NULL);
	printf("Thread %d joined\n", tid);
	exited_processes_count++;
	printf("Current exitted processes count %d\n", exited_processes_count); */
}

int main(void) {
	pid_t pid;
	pid_t p_ids[5];
	int i;

	(void) signal(SIGCHLD, my_handler);
	pthread_t t_ids[5];

	for (i = 0; i < 5; i++) {
		if ((pid = fork()) < 0) {
			printf("Can\'t fork child %d\n", i);
			exit(1);
		} else if (pid == 0) {
			exit(200 + i);
		} else {
		}

		p_ids[i] = pid;
	}

	if (pid > 0) {
		for (i = 0; i < 5; i++) {
			pthread_create(&t_ids[i], NULL, &thread_func, (void*) (p_ids + i));
			// printf("Thread waiting for process %d created\n", pid);
		}

		for (i = 0; i < 5; i++) {
			pthread_join(t_ids[i], NULL);
			// printf("Thread waiting for process %d joined\n", pid);
		}
	}

	while (1);
	return 0;

}
