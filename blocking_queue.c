#include "blocking_queue.h"

static struct blocking_queue *
$fx (blocking_queue, uninit)
{
  pthread_cond_destroy (&this->cond);
  pthread_mutex_destroy (&this->lock);
  lsfree$ (&this->head, &this->tail);
  return this;
}

static void
unlock_wrapper (void *mutex)
{
  pthread_mutex_unlock (mutex);
}

xapr *
$fx (blocking_queue, shift)
{
  xapr *ret;
  pthread_cleanup_push (unlock_wrapper, &this->lock);
  pthread_mutex_lock (&this->lock);
  while (this->head == null && this->tail == null)
    pthread_cond_wait (&this->cond, &this->lock);
  ret = list$ (shift) (&this->head, &this->tail);
  pthread_mutex_unlock (&this->lock);
  pthread_cleanup_pop (0);
  return ret;
}

void
$fx (blocking_queue, push, xapr *node)
{
  pthread_mutex_lock (&this->lock);
  list$ (push) (node, &this->head, &this->tail);
  pthread_mutex_unlock (&this->lock);
  pthread_cond_signal (&this->cond);
}

struct blocking_queue *
$fx (blocking_queue, init)
{
  this->head = this->tail = null;
  pthread_mutex_init (&this->lock, null);
  pthread_cond_init (&this->cond, null);
  bind$ (blocking_queue, uninit, this);
  return this;
}
