# Simple but leet hash function (level06)

This [binary](source.c) takes a login into input and then a "serial" value. A
kind of hashing function is then applied on the login string and the resulting
value is compared against the serial given by the user. If they are equal, we
get a shell.

There is a ptrace call that somewhat makes the job harder, but it's really
simple anyway. All we have to do is compile an other binary that will just
execute the same hash function and print the resulting value instead of just
comparing it to user input.

The hash program:

```C
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
```

Compile and run with any valid login:

```shell
# compile it in 32bit
gcc -m32 hash.c -o hash
# run it
./hash
-> Enter Login: yforeau
# our serial value
serial for yforeau: 6234040
```

Then run the original binary with these values:

```shell
# run the binary
./level06
***********************************
*               level06           *
***********************************
# input the login
-> Enter Login: yforeau
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
# input the serial
-> Enter Serial: 6234040
Authenticated!
# and you get a shell
$ whoami
level07
```
