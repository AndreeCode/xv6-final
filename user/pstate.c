#include "kernel/types.h"
#include "user/user.h"

// Same order as enum procstate in kernel/proc.h
static char *names[] = { "unused", "used", "sleeping", "runnable",
                         "running", "zombie", "suspended" };

// pstate [pid...]: shows the state of the given processes,
// or of all processes if no pid is given.
int
main(int argc, char *argv[])
{
  int i, st;

  printf("PID\tESTADO\n");
  if (argc < 2) {
    for (i = 1; i < 200; i++)
      if ((st = pstate(i)) >= 0)
        printf("%d\t%s\n", i, names[st]);
    exit(0);
  }
  for (i = 1; i < argc; i++) {
    if ((st = pstate(atoi(argv[i]))) < 0)
      printf("%s\tno existe\n", argv[i]);
    else
      printf("%s\t%s\n", argv[i], names[st]);
  }
  exit(0);
}
