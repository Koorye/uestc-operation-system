#include "buffer_pool.h"

#include <semaphore.h>
#include <stdio.h>

void init_pool(struct buffer_pool *pool, int size) {
  pool->buf = (char *)malloc(MAX_POOL_SIZE * sizeof(char) + 16);
  pool->put_pos = 0;
  pool->get_pos = 0;
  pool->size = size;
  sem_init(&pool->empty, 0, size);
  sem_init(&pool->full, 0, 0);
  sem_init(&pool->mutex, 0, 1);
}

int put(struct buffer_pool *pool, char c) {
  if (pool->put_pos > MAX_POOL_SIZE) {
    printf("缓冲区已耗尽\n");
    return EOF;
  } else if (pool->put_pos - pool->get_pos >= pool->size) {
    printf("缓冲区已满\n");
    return -2;
  }
  pool->buf[pool->put_pos++] = c;
  return 0;
}

char get(struct buffer_pool *pool) {
  if (pool->get_pos > MAX_POOL_SIZE) {
    printf("缓冲区已耗尽\n");
    return EOF;
  } else if (pool->get_pos >= pool->put_pos) {
    printf("缓冲区为空\n");
    return NULL;
  }
  return pool->buf[pool->get_pos++];
}
