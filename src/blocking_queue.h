#pragma once
#ifndef _H_BLOCKING_QUEUE_
#define _H_BLOCKING_QUEUE_ 1

#include "cmacs.h"
#include <pthread.h>

class$ (blocking_queue, {
  xapr *mbr$ (head);
  xapr *mbr$ (tail);
  pthread_mutex_t mbr$ (lock);
  pthread_cond_t mbr$ (cond);
});

xapr *$fx (blocking_queue, shift);

int $fx (blocking_queue, push, xapr *node);

static inline struct
{
  xapr *fx$ (shift);
  int fx$ (push, xapr *node);
} *
$fn (blocking_queue, vtab) ()
{
  static typeof (*$fn (blocking_queue, vtab) ()) vtab = {
    .fn$ (shift) = (void *)$fn (blocking_queue, shift),
    .fn$ (push) = (void *)$fn (blocking_queue, push),
  };
  return &vtab;
}

struct blocking_queue *$fx (blocking_queue, init);

#endif /* _H_BLOCKING_QUEUE_ */
