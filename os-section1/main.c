#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#include "philosopher.h"

int main() {
  // 初始化信号量 allow_num
  sem_t allow_num;
  sem_init(&allow_num, 0, ALLOWED_NUM);

  // 初始化信号量，将 chopsticks 数组的每个元素设为 1
  sem_t chopsticks[CHOPSTICK_NUM];
  for (int i = 0; i < CHOPSTICK_NUM; ++i) {
    sem_init(&chopsticks[i], 0, 1);
  }

  sem_t mutex;
  sem_init(&mutex, 0, 1);

  // 构造哲学家数量相当的线程，并通过结构体传参
  pthread_t thread[PHILOSOPHER_NUM];
  philosopher_arg arg[PHILOSOPHER_NUM];

  // 构造当前进食的哲学家数组，若进食，则将对应下标设为 1
  int eating_philosophers[PHILOSOPHER_NUM];
  for (int i = 0; i < PHILOSOPHER_NUM; ++i) {
    eating_philosophers[i] = 0;
  }

  // 开启每个哲学家线程
  for (int i = 0; i < PHILOSOPHER_NUM; ++i) {
    arg[i].id = i + 1;
    arg[i].chopsticks = chopsticks;
    arg[i].allowed_num = &allow_num;
    arg[i].mutex = &mutex;
    arg[i].eating_philosophers = &eating_philosophers;
    pthread_create(&thread[i], NULL, &run, &arg[i]);
  }

  // 不结束主线程
  while (1) {
    sched_yield();
  }

}