#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid, result;
  if (argc != 2) {
    fprintf(2, "Uso: suspend PID\n");
    exit(1);
  }
  for (char *s = argv[1]; *s; s++) {
    if (*s < '0' || *s > '9') {
      fprintf(2, "PID invalido\n");
      exit(1);
    }
  }
  pid = atoi(argv[1]);
  if (pid <= 0) {
    fprintf(2, "PID invalido\n");
    exit(1);
  }
  result = suspend(pid);
  if (result == 0)
    printf("Solicitud de suspension aceptada: PID %d\n", pid);
  else
    fprintf(2, "Error en suspend PID %d: codigo %d\n", pid, result);
  exit(result == 0 ? 0 : 1);
}
