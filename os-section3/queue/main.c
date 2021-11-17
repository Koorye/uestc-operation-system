#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
  long type;
  char content[1024];
} Msg;

int main() {
  key_t key = ftok("./tmp", 66);
  int id = msgget(key, IPC_CREAT | 0666);
  if (id == -1) {
    perror("消息队列创建失败");
  }

  pid_t p1 = fork();
  if (p1 < 0) {
    perror("子进程创建失败");
  } else if (p1 == 0) {
    // 子进程 1 逻辑
    Msg msg1;
    msg1.type = 1;
    char *str = "Child process 1 is sending a message!\n";
    strcpy(msg1.content, str);
    msgsnd(id, &msg1, 512, 0);
    return 0;
  }

  pid_t p2 = fork();
  if (p2 < 0) {
    perror("子进程创建失败");
  } else if (p2 == 0) {
    // 子进程 2 逻辑
    Msg msg1;
    msg1.type = 2;
    char *str = "Child process 2 is sending a message!\n";
    strcpy(msg1.content, str);
    msgsnd(id, &msg1, 512, 0);
    return 0;
  }


  // 父进程逻辑
  waitpid(p1, NULL, 0);
  waitpid(p2, NULL, 0);
  Msg res;
  msgrcv(id, &res, 512, 1, 0);
  printf("%s", res.content);
  msgrcv(id, &res, 512, 2, 0);
  printf("%s", res.content);
  return 0;
}