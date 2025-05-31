#pragma once
#ifndef _H_THRDPOOL_
#define _H_THRDPOOL_ 1

#include "cmacs.h"
#include "blocking_queue.h"

class$ (thrdpool, {
  struct blocking_queue mbr$ (exec_queue);
  struct blocking_queue mbr$ (cancel_queue);
  pthread_t *mbr$ (tid);
  xapr mbr$ (tail);
  unsigned size;
});

int $fx (thrdpool, exec, void (*fn) (void *), void *arg);

unsigned $fx (thrdpool, resize, unsigned size);

static inline struct
{
  int fx$ (exec, void (*fn) (void *), void *arg);
  unsigned fx$ (resize, unsigned size);
} *
$fn (thrdpool, vtab) ()
{
  static typeof (*$fn (thrdpool, vtab) ()) vtab = {
    .fn$ (exec) = (void *)$fn (thrdpool, exec),
    .fn$ (resize) = (void *)$fn (thrdpool, resize),
  };
  return &vtab;
}

struct thrdpool *$fx (thrdpool, init);

#endif /* _H_THRDPOOL_ */
