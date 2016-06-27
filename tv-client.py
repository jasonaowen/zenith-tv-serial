import zmq
import commands_pb2

command = commands_pb2.Command()
command.get_power.SetInParent()

context = zmq.Context()

print("Connecting to server...")
socket = context.socket(zmq.REQ)
socket.connect("ipc:///tmp/tv-serial.sock")

socket.send(
  command.SerializeToString()
)

raw_result = socket.recv()
command_result = commands_pb2.CommandResult()
command_result.ParseFromString(raw_result)
print("Received result: {}".format(command_result))
