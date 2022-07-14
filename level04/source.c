#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <strings.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>

int		main(void)
{
	pid_t	pid;
	int		status = 0;
	long	mysterious_value = 0;
	char	input[128] = { 0 };

	if (!(pid = fork())) {
		prctl(PR_SET_PDEATHSIG, SIGHUP);
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		puts("Give me some shellcode, k");
		gets(input);
	} else {
		while (mysterious_value != 0xb) {
			wait(&status);
			if (WIFEXITED(status) || WIFSIGNALED(status)) {
				puts("child is exiting...");
				return (0);
			}
			mysterious_value = ptrace(PTRACE_PEEKUSER, pid, (void *)0x2c, NULL);
		}
		puts("no exec() for you");
		kill(pid, SIGKILL);
	}
	return (0);
}

