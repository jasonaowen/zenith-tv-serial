#!/usr/bin/env python

import argparse
import commands_pb2
import sys
import zmq


def parse_args():
    description = (
            'Send a command to the daemon controlling the television '
            'and print its reponse.')
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('command', choices=['power', 'input'], help='Command to send')
    parser.add_argument('argument', choices=['on', 'off', 'status', 'AV1', 'PC', 'HDMI1', 'HDMI2'])
    parser.add_argument('--verbose', '-v', action='count', help='Log more details. Only one level currently.')
    return parser.parse_args()

def connect_to_server():
    print("Connecting to server...")
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect("ipc:///tmp/tv-serial.sock")
    return socket


def build_command_message(command, argument):
    message = commands_pb2.Command()
    if command == 'power':
        if argument == 'status':
            message.get_power.SetInParent()
        elif argument == 'on':
            message.set_power.SetInParent()
            message.set_power.power_state = commands_pb2.POWER_ON
        elif argument == 'off':
            message.set_power.SetInParent()
            message.set_power.power_state = commands_pb2.POWER_OFF
    elif command == 'input':
        if argument == 'status':
            message.get_input.SetInParent()
        elif argument in ['AV1', 'PC', 'HDMI1', 'HDMI2']:
            message.set_input.SetInParent()
            message.set_input.input_state = getattr(commands_pb2, argument)
    return message


def print_result(raw_result):
    command_result = commands_pb2.CommandResult()
    command_result.ParseFromString(raw_result)
    print("Received result: {}".format(command_result))


def main():
    args = parse_args()
    command_message = build_command_message(args.command, args.argument)
    socket = connect_to_server()
    print("Sending message: {}".format(command_message))
    socket.send(
        command_message.SerializeToString()
    )
    print_result(socket.recv())


if __name__ == "__main__":
    status = main()
    sys.exit(status)
