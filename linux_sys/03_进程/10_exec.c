#include <stdio.h>
#include <unistd.h>

/*
exec函数族
有六种以exec开头的函数，统称exec函数：
int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execle(const char *path, const char *arg, ..., char *const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execve(const char *path, char *const argv[], char *const envp[]);
*/

/*
exec函数族一般规律
exec函数一旦调用成功即执行新的程序，不返回。只有失败才返回，错误值-1。所以通常我们直接在exec函数调用后直接调用perror()和exit()，无需if判断。
l (list)			命令行参数列表
p (path)			搜素file时使用path变量
v (vector)			使用命令行参数数组
e (environment)	使用环境变量数组,不使用进程原有的环境变量，设置新加载程序运行的环境变量
事实上，只有execve是真正的系统调用，其它五个函数最终都调用execve，所以execve在man手册第2节，其它函数在man手册第3节。
*/

int main()
{
  //execlp加载一个进程，使用PATH环境变量（即如果命令位于PATH环境变量的路径下,就不需要指定路径）
  // execlp("ls", "ls", "-l", "-F", NULL);
  
  //execl加载一个进程，使用路径+程序名来加载
  //execl("../test/hello.out", "hello.out", "5", "helloworld", NULL);
  
  //execvp加载一个进程，使用字符串数组方式传递参数
  //char* argv[] = {"ls", "-l", "-F", NULL};
  //execvp("/bin/ls", argv);

      printf("========================\n");

    char *argvv[] = {"ls", "-l", "-F", "R", "-a", NULL};
    

    pid_t pid = fork();
    if (pid == 0) {
        execl("/bin/ls", "ls", "-l", "-F", "-a", NULL);
        execv("/bin/ls", argvv);
        perror("execlp");
        exit(1);

    } else if (pid > 0) {
        sleep(1);
        printf("parent\n");
    }

  return 0;
}
