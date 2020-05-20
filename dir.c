#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_file_mode(mode_t mode) {
	printf((S_ISDIR(mode)) ? "d" : "-");
	printf((mode & S_IRUSR) ? "r" : "-");
	printf((mode & S_IWUSR) ? "w" : "-");
	printf((mode & S_IXUSR) ? "x" : "-");
	printf((mode & S_IRGRP) ? "r" : "-");
	printf((mode & S_IWGRP) ? "w" : "-");
	printf((mode & S_IXGRP) ? "x" : "-");
	printf((mode & S_IROTH) ? "r" : "-");
	printf((mode & S_IWOTH) ? "w" : "-");
	printf((mode & S_IXOTH) ? "x" : "-");
	printf(" ");
}

int main(int argc, char **argv) {
	char *path = argv[1];

	if (argc < 2) {
		path = "./";
	}

	DIR* dir = opendir(path);
	struct dirent *currentdir;
	char fullpath[256];
	struct stat filestat;

	while ((currentdir = readdir(dir)) != NULL) {
		// strcpy(fullpath, path);
		// printf("Before strcat, %s, %s\n", fullpath, currentdir->d_name);
		// strcat(fullpath, currentdir->d_name);
		snprintf(fullpath, 256, "%s%s", path, currentdir->d_name);
		// printf("%s\n", fullpath);

		if (stat(fullpath, &filestat) == -1) {
			perror("stat");
			exit(-1);
		}

		print_file_mode(filestat.st_mode);
		printf("%ld %s %s %ld %.19s %s\n", filestat.st_nlink,
			getpwuid(filestat.st_uid)->pw_name,
			getgrgid(filestat.st_gid)->gr_name,
			filestat.st_size, ctime(&filestat.st_mtime), currentdir->d_name);
	}

	return 0;
}
