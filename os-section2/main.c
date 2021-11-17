#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#include "buffer_pool.h"
#include "consumer.h"
#include "producer.h"

int main(int argc, char const *argv[]) {
  struct buffer_pool pool;
  init_pool(&pool, 5);

  pthread_t producer[3];
  pthread_t consumer[4];

  for (int i = 0; i < 3; ++i) {
    pthread_create(&producer[i], NULL, &run_produce, &pool);
  }

  for (int i = 0; i < 4; ++i) {
    pthread_create(&consumer[i], NULL, &run_consume, &pool);
  }

  while (1) {
    sched_yield();
  }
}
