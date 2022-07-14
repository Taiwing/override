#include <stdio.h>
#include <string.h>
#include <strings.h>

char	a_user_name[0x100];

int		verify_user_name(void)
{
	puts("verifying username....\n");
	return strncmp(a_user_name, "dat_wil", 7);
}

int		verify_user_pass(char *pass)
{
	return strncmp(pass, "admin", 5);
}

int		main(void)
{
	char	input[76];
	int		result, ret = 1;

	bzero(input, sizeof(input));
	puts("********* ADMIN LOGIN PROMPT *********");
	printf("Enter Username: ");
	fgets(a_user_name, 0x100, stdin);
	if (!verify_user_name()) {
		puts("Enter Password: ");
		fgets(input, 100, stdin);
		result = verify_user_pass(input);
		if (!result || result)
			puts("nope, incorrect password...\n");
		else
			ret = 0;
	}
	else
		puts("nope, incorrect username...\n");
	return (ret);
}

