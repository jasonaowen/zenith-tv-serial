#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "commands.h"

int open_serial() {
  int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY);
  if (fd < 0) {
    perror("Error opening /dev/ttyS0");
    exit(EXIT_FAILURE);
  }
  return fd;
}

int main() {
  int fd;
  COMMAND_STATUS command_status;
  POWER power_status;
  INPUT input_status;
  fd = open_serial();
  command_status = tv_input_status(fd, &input_status);
  printf("Command status (SUCCESS): %i\n", SUCCESS);
  printf("Command status (FAILURE): %i\n", FAILURE);
  printf("Command status (TIMEOUT): %i\n", TIMEOUT);
  printf("Command status (read   ): %i\n", command_status);

  printf("Input status (PC   ): %i\n", PC);
  printf("Input status (HDMI2): %i\n", HDMI2);
  printf("Input status (read ): %i\n", input_status);
  printf("Input status (name ): %s\n", INPUT_NAMES[input_status]);
  /*
  printf("Power status (POWER_ON ): %i\n", POWER_ON);
  printf("Power status (POWER_OFF): %i\n", POWER_OFF);
  printf("Power status (read     ): %i\n", power_status);
  tv_power_on(fd);
  tv_power_off(fd);
  */


  return 0;
}
