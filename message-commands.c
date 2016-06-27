#include "build/commands.pb-c.h"
#include "message-commands.h"
#include "serial-commands.h"

void handle_get_power_command(CommandResult *reply,
                              uint8_t *reply_buffer,
                              size_t reply_buffer_length,
                              int fd
                             ) {
  COMMAND_STATUS command_status;
  POWER power;

  command_status = tv_power_status(fd, &power);
  reply->command_status = command_status;
  if (command_status == SUCCESS) {
    reply->has_power_state = 1;
    reply->power_state = power;
  }
}

size_t execute_command(Command *command, uint8_t *reply_buffer, size_t reply_buffer_length, int fd) {
  POWER power;
  INPUT input;
  COMMAND_STATUS command_status;
  CommandResult reply = COMMAND_RESULT__INIT;
  size_t reply_length;
  reply.command_status = COMMAND_STATUS__INVALID;

  if (command->get_power) {
    handle_get_power_command(&reply, reply_buffer, reply_buffer_length, fd);
  }

  reply_length = command_result__get_packed_size(&reply);
  assert(reply_buffer_length > reply_length);
  command_result__pack(&reply, reply_buffer);

  return reply_length;
}

