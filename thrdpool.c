#include "thrdpool.h"

struct thrd
{
  xapr list_entry;
  pthread_t tid;
  bool tail;
};

static struct thrdpool *
$fx (thrdpool, uninit)
{
  $call (thrdpool, resize, this, 0);
  struct thrd *thrd = malloc$ (sizeof (struct thrd));
  thrd->tail = true;
  $call (blocking_queue, push, &this->cancel_queue, &thrd->list_entry);
  pthread_detach (this->cancel_thread);
  return null;
}

struct task
{
  xapr list_entry;
  void (*fn) (void *);
  void *arg;
};

static void *
exec (void *queue)
{
  for (;;)
    {
      pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, null);
      pthread_setcanceltype (PTHREAD_CANCEL_DEFERRED, null);
      auto node = $call (blocking_queue, shift, queue);
      auto task = container_of (node, struct task, list_entry);
      pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, null);
      task->fn (task->arg);
      free (task);
    }
  pthread_exit (null);
}

unsigned
$fx (thrdpool, resize, unsigned size)
{
  if (size == this->size)
    return size;
  if (size > this->size)
    {
      this->tid = realloc (this->tid, sizeof (pthread_t) * size);
      for (unsigned i = this->size; i < size; ++i)
        pthread_create (this->tid + i, null, exec, &this->exec_queue);
    }
  else
    {
      for (unsigned i = size; i < this->size; ++i)
        {
          pthread_cancel (this->tid[i]);
          struct thrd *thrd = malloc$ (sizeof (struct thrd));
          thrd->tid = this->tid[i];
          thrd->tail = false;
          $call (blocking_queue, push, &this->cancel_queue, &thrd->list_entry);
        }
      if (size == 0)
        free (move (this->tid));
      else
        this->tid = realloc (this->tid, sizeof (pthread_t) * size);
    }
  return this->size = size;
}

void
$fx (thrdpool, exec, void (*fn) (void *), void *arg)
{
  if (!fn)
    return;
  struct task *task = malloc$ (sizeof (struct task));
  task->fn = fn;
  task->arg = arg;
  $call (blocking_queue, push, &this->exec_queue, &task->list_entry);
}

static void *
cancel (void *arg)
{
  struct thrdpool *this = arg;
  for (;;)
    {
      auto node = $call (blocking_queue, shift, &this->cancel_queue);
      auto thrd = container_of (node, struct thrd, list_entry);
      auto tail = thrd->tail;
      auto tid = thrd->tid;
      free (thrd);
      if (tail)
        break;
      pthread_join (tid, null);
    }
  call$ (&this->cancel_queue, uninit);
  call$ (&this->exec_queue, uninit);
  free (this);
  pthread_exit (null);
}

struct thrdpool *
$fx (thrdpool, init)
{
  $call (blocking_queue, init, &this->exec_queue);
  $call (blocking_queue, init, &this->cancel_queue);
  pthread_create (&this->cancel_thread, null, cancel, this);
  this->tid = null;
  this->size = 0;
  bind$ (thrdpool, uninit, this);
  return this;
}
