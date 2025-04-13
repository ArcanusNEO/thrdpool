#pragma once
#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H 1

#include "cmacs.h"
#include <pthread.h>

class$ (blocking_queue, {
  xapr *mbr$ (head);
  xapr *mbr$ (tail);
  pthread_mutex_t mbr$ (lock);
  pthread_cond_t mbr$ (cond);
});

struct blocking_queue *$fx (blocking_queue, init);

xapr *$fx (blocking_queue, shift);

void $fx (blocking_queue, push, xapr *node);

#endif
