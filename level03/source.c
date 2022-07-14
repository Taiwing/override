#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int		decrypt(int key)
{
	char	encrypted_message[] = "Q}|u`sfg~sf{}|a3";

	for (int i = 0; encrypted_message[i]; ++i)
		encrypted_message[i] ^= key;
	if (!strncmp(encrypted_message, "Congratulations!", 17))
		return (system("/bin/sh"));
	return (puts("\nInvalid Password"));
}

void test(int password)
{
	int		key = 0x1337d00d - password;

	if (key > 0 && key < 22)
		decrypt(key);
	else
		decrypt(rand());
}

int		main(void)
{
	int		password;

	srand(time(NULL));
	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &password);
	test(password);
	return (0);
}

