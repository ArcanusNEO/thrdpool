#pragma once
#ifndef _H_BLOCKING_DEQUE_
#define _H_BLOCKING_DEQUE_ 1

#include "cmacs.h"
#include <pthread.h>

class$ (blocking_deque, {
  xapr *mbr$ (head);
  xapr *mbr$ (tail);
  pthread_mutex_t mbr$ (lock);
  pthread_cond_t mbr$ (cond);
});

xapr *$fx (blocking_deque, shift);
xapr *$fx (blocking_deque, pop);
int $fx (blocking_deque, unshift, xapr *node);
int $fx (blocking_deque, push, xapr *node);

static inline struct
{
  xapr *fx$ (shift);
  xapr *fx$ (pop);
  int fx$ (unshift, xapr *node);
  int fx$ (push, xapr *node);
} *
$fn (blocking_deque, vtab) ()
{
  static typeof (*$fn (blocking_deque, vtab) ()) vtab = {
    .fn$ (shift) = (void *)$fn (blocking_deque, shift),
    .fn$ (pop) = (void *)$fn (blocking_deque, pop),
    .fn$ (unshift) = (void *)$fn (blocking_deque, unshift),
    .fn$ (push) = (void *)$fn (blocking_deque, push),
  };
  return &vtab;
}

struct blocking_deque *$fx (blocking_deque, init);

#endif /* _H_BLOCKING_DEQUE_ */
