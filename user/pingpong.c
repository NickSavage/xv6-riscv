
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int x[2];
  int y[2];

  pipe(x);
  pipe(y);
  if (fork() == 0) {
    close(x[1]);
    close(y[0]);
    char ping;
    read(x[0], &ping, 1);
    if (ping == '1') {
      printf("%d: received ping\n", getpid());
      write(y[1], "1", 1);
    }
    close(x[0]);
    close(y[1]);
    exit(0);

  } else {
    close(x[0]);
    close(y[1]);
    char pong;
    write(x[1], "1", 1);

    read(y[0], &pong, 1);

    if (pong == '1') {
      printf("%d: received pong\n", getpid());
    }

    close(x[1]);
    close(y[0]);
    exit(0);
  }
}
