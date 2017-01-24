# Remote Control
Control the Zenith Z42P3/Z50P3 Television via Serial

## Background

I recently discovered that [the manual for my television
set](http://www.manualslib.com/manual/485866/Zenith-Z42p3.html?page=51) has a
reference to using the serial port as an "external control device". Everything
the remote control can do can be done via the serial port, which opens up some
interesting automation possibilities.

## C Dependencies

You need ZeroMQ libs, the Protobuf C compiler, and Python dev files to compile the tv-serial executable
```
apt install libzmq3-dev protobuf-c-compiler python-dev
```

## Python Dependencies

You need the python ZeroMQ and the Protobuf python library and compiler to run the tv-client.py script

```
apt install protobuf-compiler
pip install pyzmq
pip install protobuf
```

## Building

Assuming you have all dependencies installed, it should be as easy as:

```
make
``` 

## License

![AGPL v3.0](https://www.gnu.org/graphics/agplv3-88x31.png)
This source code is shared under the GNU Affero General Public License, v3.0.
