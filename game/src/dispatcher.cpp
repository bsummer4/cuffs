/**
  @addtogroup Dispatcher
  @ingroup Game
  @section Dispatcher

  @verbatim
  Usage: dispatcher [interval in ms]
  where interval must be less than 1000
  @endverbatim

  Reads lines from stdin.  Every 'interval' ms, send all the lines
  that we have recieved with a interval counter at the beginning.  If no
  lines were received in a given interval, then print a single, blank
  line for that interval.

  At the end, the number of 'interval's that have passed and the
  number of times we have printed lines is outputed to standard
  output.

  @subsection Example
  @verbatim
      $ ./dispatcher 999
      hi
      0 hi
      there
      1 there
      ^D
      timing error: 2 vs 2
  @endverbatim
**/


#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "lib.h"

using namespace std;

unsigned long interval = 5; // ms
unsigned long counter = 0;
bool sending = false;
int queue_size = 0;
char *queue[1024];

void send (int signum) {
  assert(!sending); // We can't handle this pace
  sending = true;
  if (!queue_size) printf("%lu\n", counter);
  ITER (ii, 0, queue_size) {
    printf("%lu %s\n", counter, queue[ii]);
    free(queue[ii]); }
  counter++;
  queue_size = 0;
  sending = false;}

int main(int argc, char **argv) {
  if (argc > 1) interval = atoi(argv[1]);
  if (!interval) return 1;
  char buffer[1024];
  (void) signal(SIGALRM, send);
  struct timeval tv_0, tv_1;
  struct timezone ignore;
  gettimeofday(&tv_0, &ignore);

  // run
  ualarm(interval*1000, interval*1000);
  while (cin.getline(buffer, 1024)) {
    char *str = strdup(buffer);
    // We need to be sure this part will happen atomically
    queue[queue_size++] = str; }

  // report
  gettimeofday(&tv_1, &ignore);
  unsigned long intervals = timeval_diff(&tv_0, &tv_1) / (1000 * interval);
  fprintf(stderr, "timing error: %lu vs %lu\n", intervals, counter);
  return 0; }
