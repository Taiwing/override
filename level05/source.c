#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int		main(void)
{
	int		i = 0;
	char	input[100];

	fgets(input, 100, stdin);
	while (1) {
		if (strlen(input) <= i) {
			printf(input);
			exit(0);
		}
		if (input[i] >= 'A' && input[i] <= 'Z')
			input[i] ^= 0x20;
		++i;
	}
	return (0);
}
