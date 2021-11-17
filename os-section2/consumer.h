#ifndef CONSUMER_H
#define CONSUMER_H
#include "buffer_pool.h"

char consume(struct buffer_pool *pool);

void run_consume(struct buffer_pool *pool);

#endif