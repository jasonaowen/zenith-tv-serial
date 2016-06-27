#ifndef MESSAGE_COMMANDS_H
#define MESSAGE_COMMANDS_H

size_t execute_command(Command *command, uint8_t *reply_buffer, size_t reply_buffer_length, int fd);

#endif
