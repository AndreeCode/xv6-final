// Pruebas de suspend(pid), resume(pid) y pstate(pid).
#include "kernel/types.h"
#include "user/user.h"

// Values of enum procstate (kernel/proc.h)
#define SLEEPING  2
#define RUNNABLE  3
#define RUNNING   4
#define SUSPENDED 6

static int ok = 0, fail = 0;

static void
check(int cond, char *name)
{
  printf("%s: %s\n", cond ? "OK  " : "FAIL", name);
  if (cond) ok++; else fail++;
}

// Child that uses the CPU forever.
static int
spinner(void)
{
  int pid = fork();
  if (pid == 0)
    for (;;)
      ;
  return pid;
}

static void
reap(int pid)
{
  kill(pid);
  wait(0);
}

int
main(void)
{
  int pid, st;

  // 1. Suspend a running process.
  pid = spinner();
  pause(2);
  suspend(pid);
  pause(5);                       // wait for the safe point
  check(pstate(pid) == SUSPENDED, "1. suspender proceso en ejecucion");

  // 2. Resume it: it can run again.
  resume(pid);
  pause(2);
  st = pstate(pid);
  check(st == RUNNABLE || st == RUNNING, "2. reanudar proceso suspendido");

  // 3. Suspending twice fails.
  suspend(pid);
  check(suspend(pid) == -4, "3. suspender dos veces da error");
  resume(pid);
  reap(pid);

  // 4. init (pid 1) cannot be suspended.
  check(suspend(1) == -2, "4. no se puede suspender init");

  // 5. Non-existent pid.
  check(suspend(9999) == -1 && resume(9999) == -1 && pstate(9999) == -1,
        "5. pid inexistente da error");

  // 6. Resuming a process that is not suspended fails.
  pid = spinner();
  check(resume(pid) == -6, "6. reanudar proceso no suspendido da error");

  // 7. A suspended process can be killed.
  suspend(pid);
  pause(5);
  kill(pid);
  check(wait(0) == pid, "7. matar proceso suspendido");

  // 8. A sleeping process stays SLEEPING (suspension is pending)
  //    and the request can be cancelled with resume.
  pid = fork();
  if (pid == 0) {
    pause(1000);
    exit(0);
  }
  pause(2);
  check(suspend(pid) == 0 && pstate(pid) == SLEEPING && resume(pid) == 0,
        "8. suspension pendiente de proceso dormido");
  reap(pid);

  printf("\n%d OK, %d FAIL\n", ok, fail);
  exit(fail != 0);
}
