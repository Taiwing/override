#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv)
{
	int	n = 0;

	if (argc > 1)
		n = atoi(argv[1]);
	else
		exit(1);
	printf("%d * 4 = %d\n", n, n * 4);
	return (0);
}
