#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#include "buffer_pool.h"

char read() {
  FILE* fp = fopen("../file.txt", "r");
  char c;
  if ((c = fgetc(fp)) == EOF) {
    return EOF;
  }

  char buf[1024];
  buf[0] = '\0';
  char tmp;
  int index = 0;
  while ((tmp = fgetc(fp)) != EOF) {
    buf[index++] = tmp;
    buf[index] = '\0';
  }
  fclose(fp);

  fp = fopen("../file.txt", "w");
  fputs(buf, fp);
  fclose(fp);
  return c;
}

int produce(struct buffer_pool* pool) {
  char c = read();
  if (c == EOF) {
    return EOF;
  }
  printf("生产者[%u] 生产字符：%c\n", pthread_self(), c);
  return put(pool, c);
}

void run_produce(struct buffer_pool* pool) {
  while (1) {
    sem_wait(&pool->empty);
    sem_wait(&pool->mutex);
    int res = produce(pool);
    if (res == EOF) {
      printf("没有字符可以再获取！\n");
      exit(0);
    }
    sem_post(&pool->mutex);
    sem_post(&pool->full);
    sleep(1);
  }
}