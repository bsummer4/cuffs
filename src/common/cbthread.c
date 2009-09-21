/* cbthread.c
   James S. Plank

Continuation-Based Thread Library - A non-preemptive, continuation-based thread library.
Copyright (C) 2009 James S. Plank

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Original: October, 1995
Revised: January, 2009

James S. Plank
Department of Electrical Engineering and Computer Science
University of Tennessee
Knoxville, TN 37996
plank@cs.utk.edu
*/

#include <stdio.h>
#include <stdlib.h>
#include "dllist.h"
#include "jrb.h"
#include <setjmp.h>

#define RUNNING 0
#define READY 1
#define BLOCKED 2
#define SLEEPING 3
#define ZOMBIE 4
#define JOINING 5

typedef struct thread {
  void (*function)();
  void *arg;
  int state;
  struct thread *joiner;
} Thread;

typedef struct gsem {
  int val;
  Dllist queue;
} *Gsem;

void cbthread_exit();

static Dllist Readyq = NULL;
static Dllist Zombies = NULL;
static JRB Sleepq = NULL;
static JRB FakeSleepq = NULL;
static double FakeTime = 0;

static int first_time = 1;
static jmp_buf thebuf;
static int debug = 0;

Thread *cbthread_self = NULL;
static Thread *Joinall = NULL;

void cbthread_set_debug(int i)
{
  debug = i;
}

static void cbthread_initialize()
{
  if (Readyq != NULL) {
    fprintf(stderr, "PT: Called cbthread_initialize twice\n");
    exit(1);
  }
  Readyq = new_dllist();
  Zombies = new_dllist();
  Sleepq = make_jrb();
  FakeSleepq = make_jrb();
  cbthread_self = (Thread *) malloc(sizeof(Thread));
  cbthread_self->state = RUNNING;
  cbthread_self->joiner = NULL;
}

void *cbthread_fork(void (*function)(), void *arg)
{
  Thread *p;

  if (Readyq == NULL) cbthread_initialize(); 

  p = (Thread *) malloc(sizeof(Thread));
  p->state = READY;
  p->function = function;
  p->arg = arg;
  p->joiner = NULL;

  dll_append(Readyq, new_jval_v((void *) p));
  if (debug) fprintf(stderr, "0x%x: Calling cbthread_fork(0x%x, 0x%x): 0x%x\n",
              cbthread_self, function, arg, p); 
  return (void *) p;
}

static void block_myself()
{
  Dllist d;
  Thread *p;
  void (*function)();
  void *arg;
  long t;

  if (Readyq == NULL) cbthread_initialize();

  if (debug) printf("0x%x: Block_myself %d\n", cbthread_self, first_time);
  /* Always longjmp down to pop all thread frames off the stack */

  if (first_time) {
    first_time = 0;
    setjmp(thebuf);
  } else {
    if (debug) printf("Doing longjmp\n");
    longjmp(thebuf, 1);
  }

  /* If the sleep queue is not empty, check to see if any sleepq
     elements should come off of the queue */

  if (!jrb_empty(Sleepq)) {
    t = time(0);
    while(!jrb_empty(Sleepq) && jrb_first(Sleepq)->key.i <= t) {
      p = (Thread *) (Sleepq->flink->val.v);
      p->state = READY;
      dll_append(Readyq, new_jval_v((void *) p));
      jrb_delete_node(jrb_first(Sleepq));
    }
  }

  /* If the ready queue is empty, now check the fake sleep queue -- if it's not empty,
     move fake time and take off the first elements */

  if (dll_empty(Readyq) && !jrb_empty(FakeSleepq)) {
    FakeTime = FakeSleepq->flink->key.d;
    while(!jrb_empty(FakeSleepq) && jrb_first(FakeSleepq)->key.d <= FakeTime) {
      p = (Thread *) (FakeSleepq->flink->val.v);
      p->state = READY;
      dll_append(Readyq, new_jval_v((void *) p));
      jrb_delete_node(jrb_first(FakeSleepq));
    }
  }

  /* Call the first thread on the ready queue */

  if (!dll_empty(Readyq)) {
    d = Readyq->flink;
    p = (Thread *) d->val.v;
    function = p->function;
    arg = p->arg;
    dll_delete_node(d);
    cbthread_self = p;
    p->state = RUNNING;
/*    if (dll_empty(Readyq) && Joinall == NULL) {
      if (debug) printf("0x%x: setting first_time to 1\n", cbthread_self);
      first_time = 1;
    }  */
    (*function)(arg);

    /* If the function returns, the thread should exit */

    cbthread_exit();

  }
 
  /* Otherwise, if there are sleepers, sleep until one of them is ready */
  else if (!jrb_empty(Sleepq)) {
    t = jrb_first(Sleepq)->key.i-t;
    sleep(t);
    block_myself();
  }
  
  /* Otherwise, there are no more threads to run.  If there is 
     a joinall continuation, call it.  Otherwise, exit */

  if (Joinall != NULL) {
    p = Joinall;
    p->state = READY;
    dll_append(Readyq, new_jval_v((void *) p));

    Joinall = NULL;
    block_myself();
  }

  /* fprintf(stderr, "No more threads to run\n"); */
  exit(0);
}

void cbthread_join(Thread *thread, void (*function)(), void *arg)
{
  if (Readyq == NULL) cbthread_initialize();

  if (thread->joiner != NULL) {
    fprintf(stderr, "Called cbthread_join on a thread twice\n");
    exit(1);
  }

  /* If the thread is a zombie -- free it and go directly to the 
     continuation */

  cbthread_self->function = function;
  cbthread_self->arg = arg;

  if (thread->state == ZOMBIE) {
    dll_delete_node((Dllist) thread->arg);
    free(thread);
    cbthread_self->state = READY; /* Unnecessary -- see P() */
    dll_prepend(Readyq, new_jval_v((void *) cbthread_self));
  
  /* Otherwise, block the thread as joining */

  } else {
    thread->joiner = cbthread_self;
    cbthread_self->state = JOINING;
  }

  block_myself();
}

void cbthread_joinall(void (*function)(), void *arg)
{
  if (Readyq == NULL) {
    cbthread_initialize();
  }

  while (!dll_empty(Zombies)) {
    free(Zombies->flink->val.v);
    dll_delete_node(Zombies->flink);
  }
  
  cbthread_self->function = function;
  cbthread_self->arg = arg;
  cbthread_self->state = JOINING;
  Joinall = cbthread_self;
  block_myself();
}

int cbthread_gsem_getval(Gsem g)
{
  return g->val;
}

void *cbthread_make_gsem(int initval) 
{
  Gsem g;

  if (initval < 0) {
    fprintf(stderr, "make_gsem: initval < 0 (%d)\n", initval);
    exit(1);
  }
  g = (Gsem) malloc(sizeof(struct gsem));
  g->val = initval;
  g->queue = new_dllist();
  return g;
}
  
void cbthread_kill_gsem(Gsem g)
{
  Dllist d;

  if (!dll_empty(g->queue)) {
    fprintf(stderr, "Killing a semahpore with threads waiting\n");
    exit(1);
  }
  free_dllist(g->queue);
  free(g);
}
  
void cbthread_gsem_P(Gsem g, void (*function)(), void *arg) 
{
  Thread *p;

  if (Readyq == NULL) cbthread_initialize();

  g->val--;

  p = cbthread_self;
  p->function = function;
  p->arg = arg;

  /* If blocking, put the continuation on the semaphore's queue, otherwise
     put the continuation on the front of the ready_queue, and call
     block_myself().  The reason for this is to pop off all the stack 
     frames and start anew */

  if (g->val < 0) {
    dll_append(g->queue, new_jval_v((void *) p));
    p->state = BLOCKED;
    if (debug) fprintf(stderr, "0x%x: blocking on semaphore 0x%x\n",
                       cbthread_self, g);
  } else {
    dll_prepend(Readyq, new_jval_v((void *) p));
    p->state = READY; /* This is not really necessary, since it's going 
                         on the head of the queue */
    if (debug) fprintf(stderr, "0x%x: P called but no blocking on 0x%x\n",
                       cbthread_self, g);
  }
  block_myself();
}

void cbthread_gsem_V(Gsem g) 
{
  Thread *p;

  if (Readyq == NULL) cbthread_initialize();

  g->val++;

  /* If g->val <= 0, unblock a thread */

  if (g->val <= 0) {
    p = (Thread *) g->queue->flink->val.v;
    dll_delete_node(g->queue->flink);
    dll_append(Readyq, new_jval_v((void *) p));
    p->state = READY;
    if (debug) fprintf(stderr, "0x%x: V called on  0x%x -- waking up 0x%x\n",
                       cbthread_self, g, p);
  } else {
    if (debug) fprintf(stderr, "0x%x: V called on  0x%x no one to wake\n",
                       cbthread_self, g);
  }
}

double cbthread_get_fake_time() {
  return FakeTime;
}


/* cbthread_sleep -- if sec <= 0, put the thread at the end of the ready queue.
   If sec > 0, put the thread with the proper waking time into the 
   sleep queue */

void cbthread_sleep(int sec, void (*function)(), void *arg)
{
  long t;
  Thread *p;
  
  if (Readyq == NULL) cbthread_initialize();

  p = cbthread_self;
  p->function = function;
  p->arg = arg;

  if (sec <= 0) {
    dll_append(Readyq, new_jval_v((void *) p));
    p->state = READY;
  } else {
    t = time(0)+sec;
    jrb_insert_int(Sleepq, t, new_jval_v((void *)p));
    p->state = SLEEPING;
  }
  block_myself();
}  

void cbthread_fake_sleep(double sec, void (*function)(), void *arg)
{
  double t;
  Thread *p;
  
  if (Readyq == NULL) cbthread_initialize();

  p = cbthread_self;
  p->function = function;
  p->arg = arg;

  if (sec <= 0) {
    dll_append(Readyq, new_jval_v((void *) p));
    p->state = READY;
  } else {
    t = FakeTime+sec;
    jrb_insert_dbl(FakeSleepq, t, new_jval_v((void *)p));
    p->state = SLEEPING;
  }
  block_myself();
}  

void cbthread_yield(void (*function)(), void *arg)
{
  cbthread_sleep(0, function, arg);
}

void cbthread_exit()
{
  Thread *p;

  /* If the thread should exit -- if there is
     a joiner, put it back on the ready queue and free yourself.
     Otherwise, become a zombie */

  if (debug) { fprintf(stderr, "0x%x: Exiting\n", cbthread_self); }

  if (cbthread_self->joiner != NULL) {
    p = cbthread_self->joiner;
    p->state = READY;
    dll_append(Readyq, new_jval_v((void *) p));
    free(cbthread_self);

  } else if (Joinall != NULL) {
    free(cbthread_self);

  } else {
    cbthread_self->state = ZOMBIE;
    dll_append(Zombies, new_jval_v((void *) cbthread_self));
    cbthread_self->arg = (void *) (Zombies->blink);
  }

  block_myself();
}

extern void cbthread_reset_stack()
{
  first_time = 1;
}
