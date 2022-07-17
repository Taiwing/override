#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ptrace.h>

uint32_t	auth(char *login)
{
	size_t		len;
	uint32_t	hash;

	login[strcspn(login, "\n")] = '\0';
	if ((len = strnlen(login, 32)) < 6)
		exit(1);
	hash = ((int)login[3] ^ 0x1337U) + 0x5eeded;
	for (int i = 0; i < (int)len; ++i) {
		if (login[i] < ' ')
			exit(1);
		hash += ((int)login[i] ^ hash) % 0x539;
	}
	return (hash);
}

int		main(void)
{
	char	login[32];

	printf("-> Enter Login: ");
	fgets(login, 32, stdin);
	printf("serial for %s: %u\n", login, auth(login));
	return (0);
}

