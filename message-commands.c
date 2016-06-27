#include "build/commands.pb-c.h"
#include "message-commands.h"
#include "serial-commands.h"

size_t execute_command(Command *command, uint8_t *reply_buffer, size_t reply_buffer_length, int fd) {
  POWER power;
  INPUT input;
  COMMAND_STATUS command_status;
  CommandResult reply = COMMAND_RESULT__INIT;
  size_t reply_length;
  reply.command_status = COMMAND_STATUS__INVALID;

  if (command->get_power) {
    command_status = tv_power_status(fd, &power);
    reply.command_status = command_status;
    if (command_status == SUCCESS) {
      reply.has_power_state = 1;
      reply.power_state = power;
    }
  }

  reply_length = command_result__get_packed_size(&reply);
  assert(reply_buffer_length > reply_length);
  command_result__pack(&reply, reply_buffer);

  return reply_length;
}

