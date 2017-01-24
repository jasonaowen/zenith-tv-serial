#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <syslog.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "serial-commands.h"

size_t trim_whitespace(char *out, size_t len, const char *in)
{
  size_t out_len, in_len = strlen(in);
  const char *end = in + in_len;

  memset(out, 0, len);

  while (isspace((unsigned char)*in)) {
    in++;
  }

  while (end > in && isspace((unsigned char)*(end - 1))) {
    end--;
  }

  out_len = (end - in);
  memcpy(out, in, out_len);

  return out_len;
}

void write_command(int fd, const char* cmd) {
  ssize_t written;
  char trimmed_cmd[100];
  written = write(fd, cmd, strlen(cmd));
  trim_whitespace(trimmed_cmd, 100, cmd);
  printf("Wrote command: '%s'; %zi of %zi bytes written.\n", trimmed_cmd, written, strlen(cmd));
}

struct Response {
  char partial_command;
  uint8_t television_id;
  char status[2];
  int data;
};

struct Response parse_response(char buf[]) {
  struct Response response;
  int scan_result;
  scan_result = sscanf(
    buf,
    " %c %d %2c%dx ",
    &response.partial_command,
    &response.television_id,
    response.status,
    &response.data);
  if (4 == scan_result) {
    printf("Command: %c\n", response.partial_command);
    printf("Television ID: %i\n", response.television_id);
    printf("Status: %.2s\n", response.status);
    printf("Data: %i\n", response.data);
  } else {
    perror("error parsing result string");
  }

  return response;
}

void print_detailed_response(char *response, int response_bytes) {
  int i;
  char trimmed_response[100];

  trim_whitespace(trimmed_response, 100, response);
  printf("Read %zi bytes of response\n", response_bytes);
  printf("Response: '%s'\n", trimmed_response);
  printf("Raw response: '");
  for (i = 0; i < response_bytes; i++) {
    printf("%02x", (unsigned)response[i]);
  }
  printf("'\n");
}

int timed_read(int fd, char *buf, ssize_t buf_size) {
  fd_set input;
  struct timeval timeout;
  int select_result;
  int read_count = 0;
  int bytes_expected = 14;

  FD_ZERO(&input);
  FD_SET(fd, &input);

  timeout.tv_sec = 0;
  timeout.tv_usec = 50000;

  select_result = select(fd+1, &input, NULL, NULL, &timeout);
  if (select_result > 0) {
    printf("Time remaining: %i microseconds\n", timeout.tv_usec);
    printf("Is the FD ready? %i\n", FD_ISSET(fd, &input));

    memset(buf, 0, buf_size);
    do {
      read_count += read(fd, buf+read_count, buf_size);
    } while (read_count < bytes_expected);
    print_detailed_response(buf, read_count);
  } else {
    printf("No response received!\n");
  }

  return read_count;
}

int status_is_okay(struct Response response) {
  return response.status[0] == 'O'
      && response.status[1] == 'K';
}

COMMAND_STATUS verify_response(struct Response actual, struct Response expected) {
    if (status_is_okay(actual)
      && actual.partial_command == expected.partial_command
      && actual.television_id == expected.television_id
      && actual.data == expected.data) {
      return SUCCESS;
    } else {
      return FAILURE;
    }
}

COMMAND_STATUS write_and_verify_command(int fd, const char* cmd, struct Response expected) {
  char buf[32];
  struct Response actual;
  int read_result;

  write_command(fd, cmd);
  read_result = timed_read(fd, buf, 32);
  if (read_result) {
    actual = parse_response(buf);
    return verify_response(actual, expected);
  } else {
    return TIMEOUT;
  }
}

COMMAND_STATUS tv_power_on(int fd) {
  const char cmd[] = "ka 0 01\r";
  struct Response expected;

  expected.partial_command = 'a';
  expected.television_id = 0;
  expected.data = 1;

  return write_and_verify_command(fd, cmd, expected);
}

COMMAND_STATUS tv_power_off(int fd) {
  const char cmd[] = "ka 0 00\r";
  struct Response expected;

  expected.partial_command = 'a';
  expected.television_id = 0;
  expected.data = 0;

  return write_and_verify_command(fd, cmd, expected);
}

COMMAND_STATUS tv_power_status(int fd, POWER *power) {
  const char cmd[] = "ka 0 FF\r";
  char buf[32];
  struct Response response;
  int read_result;

  write_command(fd, cmd);
  read_result = timed_read(fd, buf, 32);
  if (read_result) {
    response = parse_response(buf);
    if (status_is_okay(response)) {
      if (response.data == 0) {
        *power = POWER_OFF;
        return SUCCESS;
      } else if (response.data == 1) {
        *power = POWER_ON;
        return SUCCESS;
      } else {
        return FAILURE;
      }
    } else {
      return FAILURE;
    }
  } else {
    *power = POWER_OFF;
    return TIMEOUT;
  }
}

COMMAND_STATUS tv_input_set(int fd, INPUT input) {
  const char cmd_format[] = "kb 0 %02X\r";
  char cmd_buf[32];
  struct Response expected;

  cmd_buf[31] = 0;
  snprintf(cmd_buf, 31, cmd_format, INPUT_IDS[input]);

  expected.partial_command = 'b';
  expected.television_id = 0;
  expected.data = INPUT_IDS[input];

  return write_and_verify_command(fd, cmd_buf, expected);
}

INPUT parse_input(int data) {
  INPUT i;
  for (i = 0; i < INPUT_COUNT; ++i) {
    if (INPUT_IDS[i] == data) {
      return i;
    }
  }
  return INPUT_COUNT;
}

COMMAND_STATUS tv_input_status(int fd, INPUT *input) {
  const char cmd[] = "kb 0 FF\r";
  char buf[32];
  struct Response response;
  int read_result;

  write_command(fd, cmd);
  read_result = timed_read(fd, buf, 32);
  if (read_result) {
    response = parse_response(buf);
    if (status_is_okay(response)) {
      *input = parse_input(response.data);
      return SUCCESS;
    } else {
      return FAILURE;
    }
  } else {
    return TIMEOUT;
  }
}
