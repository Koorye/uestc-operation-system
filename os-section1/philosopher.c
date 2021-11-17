#include "philosopher.h"

#include <semaphore.h>
#include <stdio.h>
#include <string.h>

// 显示当前正在进食的哲学家
void display_eating_philosophers(philosopher_arg *arg) {
  printf("当前正在进食的哲学家：");
  for (int i = 0; i < PHILOSOPHER_NUM; ++i) {
    if (*(arg->eating_philosophers + i) == 1) {
      printf("%d ", i + 1);
    }
  }
  printf("\n");
}

// 模拟进食
void eat(philosopher_arg *arg) {
  int *this = arg->eating_philosophers + arg->id - 1;

  sem_t *mutex = arg->mutex;
  sem_wait(mutex);
  *this = 1;
  printf("哲学家%d正在进食, ", arg->id);
  display_eating_philosophers(arg);
  sem_post(mutex);

  // 模拟进食时间
  usleep(1000 * 100);

  sem_wait(mutex);
  *this = 0;
  printf("哲学家%d开始思考, ", arg->id);
  display_eating_philosophers(arg);
  sem_post(mutex);
}

void run(philosopher_arg *arg) {
  int id = arg->id;
  sem_t *allowed_num = arg->allowed_num;

  sem_t *chopsticks = arg->chopsticks;
  sem_t *left_chopsticks = chopsticks + ((id - 1));
  sem_t *right_chopsticks = chopsticks + ((id) % CHOPSTICK_NUM);

  while (1) {
    sem_wait(allowed_num);
    sem_wait(left_chopsticks);
    sem_wait(right_chopsticks);
    eat(arg);
    sem_post(right_chopsticks);
    sem_post(left_chopsticks);
    sem_post(allowed_num);
  }
}