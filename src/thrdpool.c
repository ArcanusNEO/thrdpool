#include "thrdpool.h"

struct thrd
{
  xapr list_entry;
  pthread_t tid;
};

struct task
{
  xapr list_entry;
  void *arg;
  void (*fn) (void *);
};

static struct thrdpool *
$fx (thrdpool, cleanup)
{
  for (unsigned i = 0; i < this->size; ++i)
    {
      auto tid = this->mbr$ (tid)[i];
      pthread_cancel (tid);
      pthread_join (tid, null);
    }
  free (this->mbr$ (tid));
  call$ (&this->mbr$ (exec_queue), fn$ (uninit));
  call$ (&this->mbr$ (cancel_queue), fn$ (uninit));
  return this;
}

static struct thrdpool *
$fx (thrdpool, uninit)
{
  if (unlikely ($fn (blocking_deque, push) (&this->mbr$ (cancel_queue),
                                            &this->mbr$ (tail))))
    {
      pthread_cancel (this->mbr$ (cancel_thread));
      return $fn (thrdpool, cleanup) (this);
    }
  return null;
}

static void *
exec (void *queue)
{
  for (;;)
    {
      pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, null);
      pthread_setcanceltype (PTHREAD_CANCEL_DEFERRED, null);
      auto node = $fn (blocking_deque, shift) (queue);
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
      pthread_t *tid = realloc (this->mbr$ (tid), sizeof (pthread_t) * size);
      if (unlikely (!tid))
        return this->size;
      this->mbr$ (tid) = tid;
      for (unsigned i = this->size; i < size; ++i)
        if (pthread_create (tid + i, null, exec, &this->mbr$ (exec_queue)))
          return this->size = i;
    }
  else
    {
      unsigned diff = this->size - size;
      for (unsigned i = 0; i < diff; ++i)
        {
          auto cur = this->size - i;
          auto tid = this->mbr$ (tid)[cur - 1];
          pthread_cancel (tid);
          struct thrd *thrd = malloc (sizeof (struct thrd));
          if (unlikely (!thrd))
            {
              size = cur;
              break;
            }
          thrd->tid = tid;
          $fn (blocking_deque, push) (&this->mbr$ (cancel_queue),
                                      &thrd->list_entry);
        }
      if (size == 0)
        free (move (this->mbr$ (tid)));
      else if (likely (size != this->size))
        {
          pthread_t *tid
              = realloc (this->mbr$ (tid), sizeof (pthread_t) * size);
          if (likely (tid))
            this->mbr$ (tid) = tid;
        }
    }
  return this->size = size;
}

#define tpput(method, op)                                                     \
  int $fx (thrdpool, method, void (*fn) (void *), void *arg)                  \
  {                                                                           \
    if (!fn)                                                                  \
      return -1;                                                              \
    struct task *task = malloc (sizeof (struct task));                        \
    if (unlikely (!task))                                                     \
      return -1;                                                              \
    task->fn = fn;                                                            \
    task->arg = arg;                                                          \
    return $fn (blocking_deque, op) (&this->mbr$ (exec_queue),                \
                                     &task->list_entry);                      \
  }
tpput (exec, push);
tpput (preempt, unshift);
#undef tpput

static void *
cancel (void *arg)
{
  struct thrdpool *this = arg;
  for (;;)
    {
      auto node = $fn (blocking_deque, shift) (&this->mbr$ (cancel_queue));
      if (node == &this->mbr$ (tail))
        break;
      auto thrd = container_of (node, struct thrd, list_entry);
      auto tid = thrd->tid;
      free (thrd);
      pthread_join (tid, null);
    }
  free ($fn (thrdpool, cleanup) (this));
  pthread_exit (null);
}

struct thrdpool *
$fx (thrdpool, init)
{
  this->fn$ (uninit) = (void *)$fn (thrdpool, uninit);
  this->mbr$ (tid) = null;
  this->size = 0;

  if (unlikely (!$fn (blocking_deque, init) (&this->mbr$ (cancel_queue))))
    goto CANCEL_QUEUE_INIT_FAILURE;
  if (unlikely (
          pthread_create (&this->mbr$ (cancel_thread), null, cancel, this)))
    goto CANCEL_THREAD_INIT_FAILURE;
  pthread_detach (this->mbr$ (cancel_thread));
  if (unlikely (!$fn (blocking_deque, init) (&this->mbr$ (exec_queue))))
    goto EXEC_QUEUE_INIT_FAILURE;

  return this;

  call$ (&this->mbr$ (exec_queue), fn$ (uninit));
EXEC_QUEUE_INIT_FAILURE:
  pthread_cancel (this->mbr$ (cancel_thread));
CANCEL_THREAD_INIT_FAILURE:
  call$ (&this->mbr$ (cancel_queue), fn$ (uninit));
CANCEL_QUEUE_INIT_FAILURE:
  return null;
}
