#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  int fd[2];
  int res = pipe(fd);
  if (res == -1) {
    perror("创建管道发生错误\n");
  }

  pid_t pid1 = fork();
  if (pid1 < 0) {
    perror("进程创建发生错误\n");
  } else if (pid1 == 0) {
    // 子进程 1 逻辑
    char *str = "Child process 1 is sending a message!\n";
    write(fd[1], str, strlen(str));
    return 0;
  }

  pid_t pid2 = fork();
  if (pid2 < 0) {
    perror("进程创建发生错误\n");
  } else if (pid2 == 0) {
    // 子进程 2 逻辑
    char *str = "Child process 2 is sending a message!\n";
    write(fd[1], str, strlen(str));
    return 0;
  }

  // 父进程逻辑
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);

  char buf[1024];
  memset(buf, 0, sizeof(buf));
  int len = read(fd[0], buf, sizeof(buf));
  printf("%s", buf);
  return 0;
}