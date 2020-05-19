#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
	char pathname[] = "file";
	char symlink_pathname[] = "symlink_";
	int symlink_pathname_length = strlen(symlink_pathname);
	char index_code[4];

	open("symlink_0", O_RDWR);
	symlink(pathname, "symlink_0");

	char *new_pathname = (char *) malloc(symlink_pathname_length + 4);
	char *old_pathname = (char *) malloc(symlink_pathname_length + 4);
	int i;
	for (i = 1; i < 128; i++) {
		// itoa(i, index_code, 10);
		snprintf(index_code, 4, "%d", i);
		// printf("Index code %s\n", index_code);

		strcpy(new_pathname, symlink_pathname);
		strcat(new_pathname, index_code);
		open(new_pathname, O_RDWR);


		// itoa(i - 1, index_code, 10);
		snprintf(index_code, 4, "%d", i - 1);
		strcpy(old_pathname, symlink_pathname);
		strcat(old_pathname, index_code);
		// printf("Iteration %d\n", i);
		// printf("Old path: %s\n", old_pathname);
		// printf("New path: %s\n\n", new_pathname);
		symlink(old_pathname, new_pathname);
	}

	printf("Iteration count: %d\n", i + 1);

	return 0;
}
