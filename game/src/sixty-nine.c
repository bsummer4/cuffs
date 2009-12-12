/**
@addtogroup sixty_nine sixty-nine
@ingroup Game

@section Description
Runs to processes with process one's stdin hooked to process two's stdout,
and visa versa. 
**/


#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

static int pid1 = -1;
static int pid2 = -1;

void kill_all(int signum) {
  if (pid1 != -1) kill(pid1, SIGTERM);
  if (pid2 != -1) kill(pid2, SIGTERM); }

void close_pipes(int *p, int *q) {
  close(p[0]); close(p[1]); close(q[0]); close(q[1]); }

// splits string by whitespace, and writes each substring to
// 'write_to'.  No characters are copied.  write_to will be NULL
// terminated
void split(char *string, char **write_to, int size) {
  write_to[size-1] = NULL;
  for (int ii = 0; ii < size-1; ii++) {
    char *current = strtok(string, " ");
    write_to[ii] = current;
    string = NULL; // strtok will remember
    if (!current) return; }}

int run (char *string) {
  char *call[20];
  split(string, call, 20);
  if (call[0] == NULL) exit(1);
  execvp(call[0], call);
  perror("execvp");
  exit(1); }

int main(int num_args, char **args) {
  assert(num_args == 3 && "usage: sixty-nine command1 command2");
  int inside[2], outside[2];
  assert(!pipe(inside));
  assert(!pipe(outside));

  pid1 = fork();
  if (!pid1) {
    dup2(outside[0], 0);
    dup2(inside[1], 1);
    close_pipes(inside, outside);
    run(args[1]); }

  pid2 = fork();
  if (!pid2) {
    dup2(inside[0], 0);
    dup2(outside[1], 1);
    close_pipes(inside, outside);
    run(args[2]); }

  close_pipes(inside, outside);
  (void) signal(SIGTERM, kill_all);
  (void) signal(SIGINT, kill_all);
  (void) signal(SIGQUIT, kill_all);
  int status1, status2, pid;
  pid = wait(&status1);
  if (pid == pid1) pid1 = -1;
  if (pid == pid2) pid2 = -1;
  pid = wait(&status2);
  return status1 | status2; }
