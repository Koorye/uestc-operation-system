#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

// 初始化常量：哲学家数量、筷子数量、允许进入房间的哲学家数量
#define PHILOSOPHER_NUM 5
#define CHOPSTICK_NUM 5
#define ALLOWED_NUM 4

#include <semaphore.h>

typedef struct {
  int id;
  sem_t *chopsticks;
  sem_t *allowed_num;
  sem_t *mutex;
  int *eating_philosophers;
} philosopher_arg;

void run(philosopher_arg *arg);

#endif