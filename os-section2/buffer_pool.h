#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H

#define MAX_POOL_SIZE 1024 * 1024

#include <semaphore.h>

struct buffer_pool {
    int size;
    char *buf;
    int put_pos;
    int get_pos;
    sem_t empty;
    sem_t full;
    sem_t mutex;
};

void init_pool(struct buffer_pool *pool, int size);

int put(struct buffer_pool *pool, char c);

char get(struct buffer_pool *pool);

#endif