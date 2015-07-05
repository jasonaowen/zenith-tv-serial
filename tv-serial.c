#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int open_serial() {
  int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY);
  if (fd < 0) {
    perror("Error opening /dev/ttyS0");
    exit(EXIT_FAILURE);
  }
  return fd;
}

void write_command(int fd) {
  const char cmd[] = "ka 0 FF\r";
  ssize_t written;

  written = write(fd, cmd, strlen(cmd));
  printf("Wrote command: %s  %zi of %zi bytes written.\n", cmd, written, strlen(cmd));
}

void read_result(int fd) {
  ssize_t readc, i;
  char buf[256];
  memset(&buf, 0, 256);
  readc = read(fd, &buf, 255);
  printf("Read %zi bytes of response: %s\n", readc, buf);
  for (i = 0; i < readc; i++) {
    printf("Byte %zi: %i (%c)\n", i, (int)buf[i], buf[i]);
  }
}

int main() {
  int fd;
  fd = open_serial();
  write_command(fd);
  read_result(fd);
  return 0;
}
