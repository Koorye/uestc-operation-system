#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#include "buffer_pool.h"

char consume(struct buffer_pool *pool) {
  char c = get(pool);
  printf("消费者[%u] 获取字符：%c\n", pthread_self(), c);
  if (c == NULL) {
    return NULL;
  } else if (c == EOF) {
    return EOF;
  }
  return c;
}

void run_consume(struct buffer_pool *pool) {
  while (1) {
    sem_wait(&pool->full);
    sem_wait(&pool->mutex);
    char res = consume(pool);
    if (res == EOF) {
      return;
    }
    sem_post(&pool->mutex);
    sem_post(&pool->empty);
    sleep(1);
  }
}