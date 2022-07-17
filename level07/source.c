#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <strings.h>

void		clear_stdin(void)
{
	int		c = 0;

	while (c != -1 && c != '\n')
		c = getchar();
}

uint32_t	get_unum(void)
{
	uint32_t	num;

	fflush(stdout);
	scanf("%u", &num);
	clear_stdin();
	return (num);
}

int			read_number(uint32_t *data)
{
	int	index;

	printf(" Index: ");
	index = get_unum();
	printf(" Number at data[%u] is %u\n", index, data[index]);
	return (0);
}

int			store_number(uint32_t *data)
{
	uint32_t	number;
	int			index;

	printf(" Number: ");
	number = get_unum();
	printf(" Index: ");
	index = get_unum();
	if ((index % 3 == 0) || (number >> 24 == 0xb7)) {
		puts(" *** ERROR! ***");
		puts("   This index is reserved for wil!");
		puts(" *** ERROR! ***");
		return (1);
	}
	data[index] = number;
	return (0);
}

int			main(int argc, char **argv, char **envp)
{
	int			ret;
	uint32_t	data[100] = { 0 };
	char		command[20] = { 0 };

	for (char **ptr = argv; *ptr; ++ptr)
		bzero(*ptr, strlen(*ptr));
	for (char **ptr = envp; *ptr; ++ptr)
		bzero(*ptr, strlen(*ptr));
	puts(
		"----------------------------------------------------\n"
		"  Welcome to wil's crappy number storage service!   \n"
		"----------------------------------------------------\n"
		" Commands:                                          \n"
		"    store - store a number into the data storage    \n"
		"    read  - read a number from the data storage     \n"
		"    quit  - exit the program                        \n"
		"----------------------------------------------------\n"
		"   wil has reserved some storage :>                 \n"
		"----------------------------------------------------\n"
	);
	while (42) {
		ret = 1;
		printf("Input command: ");
		fgets(command, sizeof(command), stdin);
		if (!strncmp(command, "store", 5))
			ret = store_number(data);
		else if (!strncmp(command, "read", 4))
			ret = read_number(data);
		else if (!strncmp(command, "quit", 4))
			return (0);
		if (!ret)
			printf(" Completed %s command successfully\n", command);
		else
			printf(" Failed to do %s command\n", command);
		bzero(command, sizeof(command));
	}
	return (0);
}
