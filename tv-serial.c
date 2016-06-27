#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <zmq.h>

#include "serial-commands.h"
#include "build/commands.pb-c.h"

const size_t MAX_MESSAGE_SIZE = 32;

int open_serial() {
  int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY);
  if (fd < 0) {
    perror("Error opening /dev/ttyS0");
    exit(EXIT_FAILURE);
  }
  return fd;
}

void *build_zmq_responder() {
  int rc;
  void *context = zmq_ctx_new();
  void *responder = zmq_socket(context, ZMQ_REP);
  rc = zmq_bind(responder, "ipc:///tmp/tv-serial.sock");
  if (rc != 0) {
    perror("Error binding to /tmp/tv-serial.sock");
    exit(EXIT_FAILURE);
  }
  printf("Listening on ipc:///tmp/tv-serial.sock...\n");

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
  zmq_send(responder, reply_buffer, reply_len, 0);

  command__free_unpacked(msg, NULL);
}

int main() {
  int serial_fd = open_serial();
  void *zmq_responder = build_zmq_responder();

  while (1) {
    handle_request(serial_fd, zmq_responder);
  }

  return 0;
}
