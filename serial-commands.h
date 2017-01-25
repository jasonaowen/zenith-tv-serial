#ifndef SERIAL_COMMANDS_H
#define SERIAL_COMMANDS_H

#include "inputs.h"

typedef enum {
  SUCCESS = 0,
  FAILURE = 1,
  TIMEOUT = 2
} COMMAND_STATUS;

typedef enum {
  POWER_OFF = 0,
  POWER_ON = 1
} POWER;

typedef enum {
  SCREENMUTE_OFF = 0,
  SCREENMUTE_ON = 1,
} SCREENMUTE;

COMMAND_STATUS tv_power_on(int fd);
COMMAND_STATUS tv_power_off(int fd);
COMMAND_STATUS tv_power_status(int fd, POWER *power);

COMMAND_STATUS tv_input_set(int fd, INPUT input);
COMMAND_STATUS tv_input_status(int fd, INPUT *input);

COMMAND_STATUS tv_screenmute_on(int fd);
COMMAND_STATUS tv_screenmute_off(int fd);
COMMAND_STATUS tv_screenmute_status(int fd, SCREENMUTE *screenmute);

#endif
