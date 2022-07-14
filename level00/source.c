#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	int	input;

	puts("***********************************");
	puts("* \t     -Level00 -\t\t  *");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &input);
	if (input != 5276)
		puts("\nInvalid Password!");
	else {
		puts("\nAuthenticated!");
		system("/bin/sh");
	}
	return (input != 5276);
}

