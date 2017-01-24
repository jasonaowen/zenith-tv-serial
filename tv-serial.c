#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <zmq.h>

#include "serial-commands.h"
#include "build/commands.pb-c.h"

const size_t MAX_MESSAGE_SIZE = 32;

int open_serial(char* device) {
  int fd = open(device, O_RDWR | O_NOCTTY);
  if (fd < 0) {
    char error[100];
    snprintf(error, 100, "Error opening %s", device);
    perror(error);
    exit(EXIT_FAILURE);
  }
  return fd;
}

void *build_zmq_responder() {
  int rc;
  mode_t previous_mode = umask(S_IXUSR | S_IXGRP | S_IXOTH);
  void *context = zmq_ctx_new();
  void *responder = zmq_socket(context, ZMQ_REP);
  rc = zmq_bind(responder, "ipc:///tmp/tv-serial.sock");
  if (rc != 0) {
    perror("Error binding to /tmp/tv-serial.sock");
    exit(EXIT_FAILURE);
  }
  printf("Listening on ipc:///tmp/tv-serial.sock...\n");
  umask(previous_mode);

  return responder;
}

void handle_request(int fd, void *responder) {
  int msg_len, reply_len;
  uint8_t command_buffer[MAX_MESSAGE_SIZE];
  uint8_t reply_buffer[MAX_MESSAGE_SIZE];
  Command *msg;

  msg_len = zmq_recv(responder, command_buffer, MAX_MESSAGE_SIZE, 0);
  assert(msg_len != -1);
  assert(msg_len < MAX_MESSAGE_SIZE);
  printf("Received message\n");
  msg = command__unpack(NULL, msg_len, command_buffer);

  reply_len = execute_command(msg, reply_buffer, MAX_MESSAGE_SIZE, fd);
  printf("Replying with message of length %i\n", reply_len);
  zmq_send(responder, reply_buffer, reply_len, 0);

  command__free_unpacked(msg, NULL);
}

int main(int argc, char **argv) {
  char* device = "/dev/ttyS0";
  if (argc > 1) { 
    device = argv[1];
  }
  int serial_fd = open_serial(device);
  void *zmq_responder = build_zmq_responder();

  while (1) {
    handle_request(serial_fd, zmq_responder);
  }

  return 0;
}
