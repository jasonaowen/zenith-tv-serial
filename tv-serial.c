#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "inputs.h"

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
  printf("Wrote command: \n%s\n  %zi of %zi bytes written.\n", cmd, written, strlen(cmd));
}

void parse_response(ssize_t len, char buf[]) {
  char cmd;
  int television_id;
  char status[2];
  int data;
  int scan_result;
  scan_result = sscanf(buf, " %c %d %2c%dx ", &cmd, &television_id, status, &data);
  if (4 == scan_result) {
    printf("Command: %c\n", cmd);
    printf("Television ID: %i\n", television_id);
    printf("Status: %.2s\n", status);
    printf("Data: %i", data);
  } else {
    perror("error parsing result string");
  }

}

void read_result(int fd) {
  ssize_t readc, i;
  char buf[32];
  memset(&buf, 0, sizeof(buf));
  readc = read(fd, &buf, sizeof(buf)-1);
  printf("Read %zi bytes of response: %s\n", readc, buf);
  for (i = 0; i < readc; i++) {
    printf("Byte %zi: %i (%c)\n", i, (int)buf[i], buf[i]);
  }
  parse_response(readc, buf);
}

int main() {
//  int fd;
  int i;
  printf("INPUT_COUNT: %i\n", INPUT_COUNT);
  for (i = 0; i < INPUT_COUNT; i++) {
    printf("Input %i named %s with data %.2i\n", i, INPUT_NAMES[i], INPUT_IDS[i]);
  }
//  fd = open_serial();
//  write_command(fd);
//  read_result(fd);
  return 0;
}
