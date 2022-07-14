#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

int		main(void)
{
	size_t	rd;
	char	password[14];
	char	flag[6];
	char	username[12];
	FILE	*stream = NULL;

	bzero(username, sizeof(username));
	bzero(flag, sizeof(flag));
	bzero(password, sizeof(password));

	if (!(stream = fopen("/home/users/level03/.pass", "r"))) {
		fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
		exit(1);
	}
	rd = fread(flag, 1, 0x29, stream);
	flag[strcspn(flag, "\n")] = '\0';
	if (rd != 0x29) {
		fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
		fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
		exit(1);
	}
	fclose(stream);

	puts("===== [ Secure Access System v1.0 ] =====");
	puts("/***************************************\\");
	puts("| You must login to access this system. |");
	puts("\\**************************************/");
	printf("--[ Username: ");
	fgets(username, 100, stdin);
	flag[strcspn(username, "\n")] = '\0';
	printf("--[ Password: ");
	fgets(password, 100, stdin);
	flag[strcspn(password, "\n")] = '\0';
	puts("*****************************************");

	if (!strncmp(flag, password, 0x29)) {
		printf("Greetings, %s!\n", username);
		system("/bin/sh");
		return (0);
	}
	printf(username);
	puts(" does not have access!");
	exit(1);
}

