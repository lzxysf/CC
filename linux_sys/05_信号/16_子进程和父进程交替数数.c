#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int n = 0;
pid_t pid;


void parent_handler(int signo)
{
	printf("%d\r\n", n);
	n += 2;

	//sleep(1);
	kill(pid, SIGUSR1);
}

void child_handler(int signo)
{
	printf("%d\r\n", n);
	n += 2;

	//sleep(1);
	kill(getppid(), SIGUSR2);
}

int main()
{
	pid = fork();

	if(pid > 0)
	{
		struct sigaction act, old_act;
		act.sa_handler = parent_handler;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGUSR2, &act, &old_act);
		
		sleep(1);

		n++;

		kill(pid, SIGUSR1);

		while(1);
	}
	else if (pid == 0)
	{
		struct sigaction act, old_act;
		act.sa_handler = child_handler;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGUSR1, &act, &old_act);

		while(1);
	}

	return 0;
}

