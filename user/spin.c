#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int i = 0;
  for (;;) {
    printf("spin %d\n", i++);
    pause(20);
  }
}
