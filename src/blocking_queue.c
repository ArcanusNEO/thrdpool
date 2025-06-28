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
  do
    pthread_testcancel ();
  while (unlikely (pthread_mutex_lock (&this->mbr$ (lock))));
  pthread_cleanup_push (mutex_unlock_wrapper, &this->mbr$ (lock));
  while (this->mbr$ (head) == null && this->mbr$ (tail) == null)
    pthread_cond_wait (&this->mbr$ (cond), &this->mbr$ (lock));
  ret = list$ (shift) (&this->mbr$ (head), &this->mbr$ (tail));
  pthread_cleanup_pop (1);
  return ret;
}

int
$fx (blocking_queue, push, xapr *node)
{
  if (unlikely (pthread_mutex_lock (&this->mbr$ (lock))))
    return -1;
  list$ (push) (node, &this->mbr$ (head), &this->mbr$ (tail));
  pthread_mutex_unlock (&this->mbr$ (lock));
  return pthread_cond_signal (&this->mbr$ (cond));
}

static void
init_cleanup (void *co, struct blocking_queue *bq) async$ (co)
{
  pthread_mutex_destroy (&bq->mbr$ (lock));
  yield$ ();
  pthread_cond_destroy (&bq->mbr$ (cond));
  exit$ ();
}

struct blocking_queue *
$fx (blocking_queue, init)
{
  this->fn$ (uninit) = (void *)$fn (blocking_queue, uninit);
  this->mbr$ (head) = this->mbr$ (tail) = null;

  int stage = 0;
  coroutine co = 0;

  do
    {
      if (unlikely (pthread_mutex_init (&this->mbr$ (lock), null)))
        break;
      ++stage;

      if (unlikely (pthread_cond_init (&this->mbr$ (cond), null)))
        break;
      ++stage;
      return this;
    }
  while (0);

  for (int i = 0; i < stage; ++i)
    init_cleanup (&co, this);
  return null;
}
