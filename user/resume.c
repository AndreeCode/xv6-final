#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid, result;
  if (argc != 2) {
    fprintf(2, "Uso: resume PID\n");
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
  result = resume(pid);
  if (result == 0)
    printf("Reanudacion aceptada: PID %d\n", pid);
  else
    fprintf(2, "Error en resume PID %d: codigo %d\n", pid, result);
  exit(result == 0 ? 0 : 1);
}
