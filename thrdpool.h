#pragma once
#ifndef THRDPOOL_H
#define THRDPOOL_H 1

#include "cmacs.h"
#include "blocking_queue.h"

class$ (thrdpool, {
  struct blocking_queue mbr$ (exec_queue);
  struct blocking_queue mbr$ (cancel_queue);
  pthread_t mbr$ (cancel_thread);
  pthread_t *mbr$ (tid);
  unsigned size;
});

struct thrdpool *$fx (thrdpool, init);

void $fx (thrdpool, exec, void (*fn) (void *), void *arg);

unsigned $fx (thrdpool, resize, unsigned size);

#endif
