#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY);
  if (fd < 0) {
    perror("Error opening /dev/ttyS0");
    return 1;
  }

  char* cmd = "ka 1 FF\r";
  char buf[256]; memset(&buf, 0, 256);
  ssize_t written, readc;
  written = write(fd, cmd, strlen(cmd));
  printf("Wrote command: %s  %i of %i bytes written.\n", cmd, written, strlen(cmd));

  readc = read(fd, &buf, 255);
  printf("Read %i bytes of response: %s\n", readc, buf);
  int i;
  for (i = 0; i < readc; i++) {
    printf("Byte %i: %i (%c)\n", i, buf[i], buf[i]);
  }

  return 0;
}
