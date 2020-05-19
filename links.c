#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
	char pathname[] = "file";

	open("links_symlink", O_RDWR);
	symlink(pathname, "links_symlink");

	open("links_link", O_RDWR);
	link(pathname, "links_link");

	return 0;
}
