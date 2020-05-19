#include <stdlib.h>
#include <fcntl.h>

int main() {
	char pathname[] = "file";
	char symlink_pathname[] = "symlink_";
	int symlink_pathname_length = strlen(symlink_pathname);
	char index_code[3];

	open("symlink_0", O_RDWR);
	symlink(pathname, "symlink_0");

	for (int i = 1; i < 128; i++) {
		itoa(i, index_code, 10);
		char *new_pathname = strcat(symlink_pathname, index_code);
		open(new_pathname, O_RDWR);


		itoa(i - 1, index_code, 10);
		char *old_pathname = strcat(symlink_pathname, index_code);

		symlink(old_pathname, new_pathname);
	}

	return 0;
}