/* cbthread.h
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

/*
   You must link with libfdr.a 

   See http://www.cs.utk.edu/~plank/plank/classes/cs360/360/notes/Libfdr/ 
       for Libfdr code and examples.
*/

/* Pointer to the currently running thread */

extern void *cbthread_self;

/* cbthread_fork says to make a new thread which runs the (*function)(arg).
   This gets put on the thread queue, and cbthread_fork returns with a 
   thread pointer.
 */

extern void *cbthread_fork(void (*function)(), void *arg);

/* cbthread_join waits for the specified thread to exit, and then calls the
              continuation.
   cbthread_joinall waits until there are no threads left on the ready queue, 
              and there are no threads sleeping.  Then its continuation
              is called.  */

extern void cbthread_join(void *tid, void (*function)(), void *arg);
extern void cbthread_joinall(void (*function)(), void *arg);

/* Pt_exit() forces the thread to exit */

extern void cbthread_exit();

/* make_gsem initializes a general semaphore with the value initval, which
   must be >= 0.  kill_gsem deallocates the semaphore.  kill_gsem() will
   fail with an error if there are any threads waiting on the semaphore. 
   With semaphores, you may access the value, but nothing else.  
   */

typedef void *cbthread_gsem;

extern cbthread_gsem cbthread_make_gsem(int initval);
extern void cbthread_kill_gsem(cbthread_gsem s);
extern int cbthread_gsem_getval(cbthread_gsem s);
    
/* gsem_P performs the P operation on the semaphore.  This operation 
   works in a special way.  You must specify a function and an argument
   with gsem_P.  If the P blocks, then when it wakes up, 
   (*function)(arg) is run.  Otherwise, (*function)(arg) is run
   directly.  In either case, gsem_P() never returns.  When 
   (*function)(arg) returns, its thread dies.
   The reason for this is that our threads are stackless.
*/

extern void cbthread_gsem_P(cbthread_gsem s, void (*function)(), void *arg);

/* gsem_V() is the normal V operation.  If the semaphore's value is 
   < 0 when gsem_V() is called, then a blocked thread is taken off
   the semaphore queue and put on the thread queue. */

extern void cbthread_gsem_V(cbthread_gsem s);


/* cbthread_sleep says to sleep for a certain number of seconds.  Then the 
   thread is put back on the ready queue.  When it is time to run the
   thread (*function)(arg) is called.     cbthread_sleep(0) is equivalent
   to cbthread_yield()  */

extern void cbthread_sleep(int sec, void (*function)(), void *arg);
extern void cbthread_yield(void (*function)(), void *arg);

extern double cbthread_get_fake_time();
extern void cbthread_fake_sleep(double seconds, void (*function)(), void *arg);

/* This is an advanced routine -- if for some reason you want to make sure that
   you do not longjmp from your current stack position, call this. */

extern void cbthread_reset_stack();
