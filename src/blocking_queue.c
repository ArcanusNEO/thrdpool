#include "blocking_queue.h"

static struct blocking_queue *
$fx (blocking_queue, uninit)
{
  pthread_cond_destroy (&this->mbr$ (cond));
  pthread_mutex_destroy (&this->mbr$ (lock));
  lsfree$ (&this->mbr$ (head), &this->mbr$ (tail));
  return this;
}

static void
mutex_unlock_wrapper (void *mutex)
{
  pthread_mutex_unlock (mutex);
}

xapr *
$fx (blocking_queue, shift)
{
  xapr *ret;
  pthread_mutex_lock (&this->mbr$ (lock));
  pthread_cleanup_push (mutex_unlock_wrapper, &this->mbr$ (lock));
  while (this->mbr$ (head) == null && this->mbr$ (tail) == null)
    pthread_cond_wait (&this->mbr$ (cond), &this->mbr$ (lock));
  ret = list$ (shift) (&this->mbr$ (head), &this->mbr$ (tail));
  pthread_cleanup_pop (1);
  return ret;
}

void
$fx (blocking_queue, push, xapr *node)
{
  pthread_mutex_lock (&this->mbr$ (lock));
  list$ (push) (node, &this->mbr$ (head), &this->mbr$ (tail));
  pthread_mutex_unlock (&this->mbr$ (lock));
  pthread_cond_signal (&this->mbr$ (cond));
}

struct blocking_queue *
$fx (blocking_queue, init)
{
  this->mbr$ (head) = this->mbr$ (tail) = null;
  pthread_mutex_init (&this->mbr$ (lock), null);
  pthread_cond_init (&this->mbr$ (cond), null);
  this->fn$ (uninit) = (void *)$fn (blocking_queue, uninit);
  return this;
}
