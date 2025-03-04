#pragma once
#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H 1

#include "cmacs.h"
#include <pthread.h>

class$ (blocking_queue, {
  xapr *head;
  xapr *tail;
  pthread_mutex_t lock;
  pthread_cond_t cond;
});

struct blocking_queue *$fx (blocking_queue, init);

xapr *$fx (blocking_queue, shift);

void $fx (blocking_queue, push, xapr *node);

#endif
