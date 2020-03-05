#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>

struct MSG
{
	int num1;
	int num2	
};

int main()
{
	int fd;
	struct MSG *msg;
	
	pid_t pid;
	pthread_mutex_t mutex;
	pthread_mutexattr_t attr;
	
	fd = open("./shared", O_RDWR|O_TRUNC|O_CREAT, 0664);
	ftruncate(fd, sizeof(struct MSG));
	if(fd == -1)
	{
		perror("open failed\r\n");
		exit(1);
	}
	msg = mmap(NULL, sizeof(struct MSG), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(msg == MAP_FAILED)
	{
		perror("mmap failed\r\n");
		exit(0);
	}
	close(fd);
	unlink("./shared");
	
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&mutex, &attr);
	pthread_mutexattr_destroy(&attr);
	
	pid = fork();
	if(pid < 0)
	{
		perror("fork error\r\n");
		exit(0);
	}
	else if(pid > 0)
	{
		int num = 100;
		while(1)
		{
			pthread_mutex_lock(&mutex);
			msg->num1 = num++;
			msg->num2 = num++;
			pthread_mutex_unlock(&mutex);
			usleep(20000);
		}
	}
	else if(pid == 0)
	{
		while(1)
		{
			pthread_mutex_lock(&mutex);
			printf("子线程中num1为%d，num2为%d\r\n", msg->num1, msg->num2);
			pthread_mutex_unlock(&mutex);
			usleep(20000);
		}
	}
	
	pthread_mutex_destroy(&mutex);

	return 0;
}
