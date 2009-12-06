#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void close_pipes(int *p, int *q) {
  close(p[0]); close(p[1]); close(q[0]); close(q[1]); }

int main(int num_args, char **args) {
  assert(num_args == 3 && "usage: sixty-nine command1 command2");
  int inside[2], outside[2];
  assert(!pipe(inside));
  assert(!pipe(outside));

  int pid1 = fork();
  if (!pid1) {
    dup2(outside[0], 0);
    dup2(inside[1], 1);
    close_pipes(inside, outside);
    return system(args[1]); }

  int pid2 = fork();
  if (!pid2) {
    dup2(inside[0], 0);
    dup2(outside[1], 1);
    close_pipes(inside, outside);
    return system(args[2]); }

  close_pipes(inside, outside);
  int status1, status2, pid;
  pid = wait(&status1);
  pid = wait(&status2);
  return status1 | status2; }
