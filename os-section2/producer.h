#ifndef PRODUCE_H
#define PRODUCE_H
#include "buffer_pool.h"

int produce(struct buffer_pool *pool);

void run_produce(struct buffer_pool *pool);

#endif