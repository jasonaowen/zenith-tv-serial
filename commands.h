#ifndef COMMANDS_H
#define COMMANDS_H

#include "inputs.h"

typedef enum {
  SUCCESS,
  FAILURE,
  TIMEOUT
} COMMAND_STATUS;

typedef enum {
  POWER_OFF,
  POWER_ON
} POWER;

COMMAND_STATUS tv_power_on(int fd);
COMMAND_STATUS tv_power_off(int fd);
COMMAND_STATUS tv_power_status(int fd, POWER *power);

COMMAND_STATUS tv_input_set(int fd, INPUT input);
COMMAND_STATUS tv_input_status(int fd, INPUT *input);

#endif
