#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void	secret_backdoor(void)
{
	char	command[128];

	fgets(command, sizeof(command), stdin);
	system(command);
}

void	set_msg(char *buffer)
{
	char	message[1024] = { 0 };

	puts(">: Msg @Unix-Dude");
	printf(">>: ");
	fgets(message, sizeof(message), stdin);
	strncpy(buffer, message, *(int *)(buffer + 180));
}

void	set_username(char *buffer)
{
	char	username[136] = { 0 };

	puts(">: Enter your username");
	printf(">>: ");
	fgets(username, 128, stdin);
	strncpy(buffer + 140, username, 41);
	printf(">: Welcome, %s", buffer + 140);
}


void	handle_msg(void)
{
	char	buffer[140];

	set_username(buffer);
	set_msg(buffer);
	puts(">: Msg sent!");
}

int		main(void)
{
	puts(
		"--------------------------------------------\n"
		"|   ~Welcome to l33t-m$n ~    v1337        |\n"
		"--------------------------------------------"
	);
	handle_msg();
	return (0);
}
