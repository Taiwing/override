#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ptrace.h>

int		auth(char *login, uint32_t serial)
{
	size_t		len;
	uint32_t	hash;

	login[strcspn(login, "\n")] = '\0';
	if ((len = strnlen(login, 32)) < 6)
		return (1);
	if (ptrace(PTRACE_TRACEME) == -1) {
		puts("\x1b[32m.---------------------------.");
		puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
		puts("\x1b[32m\'---------------------------\'");
		return (1);
	}
	hash = ((int)login[3] ^ 0x1337U) + 0x5eeded;
	for (int i = 0; i < (int)len; ++i) {
		if (login[i] < ' ')
			return (1);
		hash += ((int)login[i] ^ hash) % 0x539;
	}
	return (serial != hash);
}

int		main(void)
{
	char	login[32];
	int		serial;

	puts("***********************************");
	puts("*\t\tlevel06\t\t  *");
	puts("***********************************");
	printf("-> Enter Login: ");
	fgets(login, 32, stdin);
	puts("***********************************");
	puts("***** NEW ACCOUNT DETECTED ********");
	puts("***********************************");
	printf("-> Enter Serial: ");
	scanf("%u", &serial);
	if (!auth(login, serial)) {
		puts("Authenticated!");
		system("/bin/sh");
		return (0);
	}
	return (1);
}

