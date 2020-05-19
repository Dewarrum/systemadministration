#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char **argv) {
	char *path = argv[1];

	DIR* dir = opendir(path);
	struct dirent *currentdir;

	while ((currentdir = readdir(dir)) != NULL) {
		printf("%s", currentdir.d_name);
	}

	return 0;
}