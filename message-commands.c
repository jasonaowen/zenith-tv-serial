#include "build/commands.pb-c.h"
#include "message-commands.h"
#include "serial-commands.h"

void handle_get_power_command(CommandResult *reply, int fd) {
  COMMAND_STATUS command_status;
  POWER power;

  command_status = tv_power_status(fd, &power);
  reply->command_status = command_status;
  if (command_status == SUCCESS) {
    reply->has_power_state = 1;
    reply->power_state = power;
  }
}

void handle_set_power_command(PowerState power_state,
                              CommandResult *reply,
                              int fd
                             ) {
  COMMAND_STATUS command_status;
  POWER power;

  if (power_state == POWER_STATE__POWER_ON) {
    command_status = tv_power_on(fd);
  } else {
    command_status = tv_power_off(fd);
  }
  reply->command_status = command_status;
}

void handle_get_input_command(CommandResult *reply, int fd) {
  COMMAND_STATUS command_status;
  INPUT input;

  command_status = tv_input_status(fd, &input);
  reply->command_status = command_status;
  if (command_status == SUCCESS) {
    reply->has_input_state = 1;
    reply->input_state = INPUT_IDS[input];
  }
}

void handle_set_input_command(InputState input_state,
                              CommandResult *reply,
                              int fd
                             ) {
  COMMAND_STATUS command_status;
  INPUT input = parse_input(input_state);

  command_status = tv_input_set(fd, input);
  reply->command_status = command_status;
}

void handle_get_screenmute_command(CommandResult *reply, int fd) {
  COMMAND_STATUS command_status;
  SCREENMUTE screenmute;

  command_status = tv_screenmute_status(fd, &screenmute);
  reply->command_status = command_status;
  if (command_status == SUCCESS) {
    reply->has_screenmute_state = 1;
    reply->screenmute_state = screenmute;
  }
}

void handle_set_screenmute_command(ScreenMuteState screenmute_state,
                              CommandResult *reply,
                              int fd
                             ) {
  COMMAND_STATUS command_status;
  SCREENMUTE screenmute;

  if (screenmute_state == SCREEN_MUTE_STATE__SCREENMUTE_ON) {
    command_status = tv_screenmute_on(fd);
  } else {
    command_status = tv_screenmute_off(fd);
  }
  reply->command_status = command_status;
}

size_t execute_command(Command *command, uint8_t *reply_buffer, size_t reply_buffer_length, int fd) {
  POWER power;
  INPUT input;
  COMMAND_STATUS command_status;
  CommandResult reply = COMMAND_RESULT__INIT;
  size_t reply_length;
  reply.command_status = COMMAND_STATUS__INVALID;

  if (command->get_power) {
    handle_get_power_command(&reply, fd);
  } else if (command->set_power) {
    handle_set_power_command(
      command->set_power->power_state,
      &reply,
      fd
    );
  } else if (command->get_input) {
    handle_get_input_command(&reply, fd);
  } else if (command->set_input) {
    handle_set_input_command(
      command->set_input->input_state,
      &reply,
      fd
    );
  } else if (command->get_screenmute) {
    handle_get_screenmute_command(&reply, fd);
  } else if (command->set_screenmute) {
    handle_set_screenmute_command(
      command->set_screenmute->screenmute_state,
      &reply,
      fd
    );
  }

  reply_length = command_result__get_packed_size(&reply);
  assert(reply_buffer_length > reply_length);
  command_result__pack(&reply, reply_buffer);

  return reply_length;
}

