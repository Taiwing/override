#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void log_wrapper(FILE *dest_file, char *action, char *filename)
{
	char log [264];

	strcpy(log, action);
	snprintf(log + strlen(log), 254 - strlen(log), filename);
	log[strcspn(log, "\n")] = '\0';
	fprintf(dest_file, "LOG: %s\n", log);
}

int main(int argc, char **argv)
{
	int		fd;
	FILE	*log_file;
	FILE	*source_file;
	// dont know the size here yet (although its not uber important)
	char	buffer[1000];
	char	dest[100] = "./backups/";

	if (argc != 2)
		printf("Usage: %s filename\n", *argv);
	if ((log_file = fopen("./backups/.log", "w")) == NULL) {
		printf("ERROR: Failed to open %s\n", "./backups/.log");
		exit(1);
	}
	log_wrapper(log_file, "Starting back up: ", argv[1]);
	if ((source_file = fopen(argv[1], "r")) == NULL) {
		printf("ERROR: Failed to open %s\n", argv[1]);
		exit(1);
	}
	strncat(dest, argv[1], sizeof(dest) - strlen(dest) - 1);
	if ((fd = open(dest, 0xc1, 0x1b0)) < 0) {
		printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
		exit(1);
	}
	while (fgetc(source_file) != -1)
		write(fd, buffer, 1);
	log_wrapper(log_file, "Finished back up ", argv[1]);
	fclose(source_file);
	close(fd);
	return (0);
}
