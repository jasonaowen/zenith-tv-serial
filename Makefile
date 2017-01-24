CC=gcc
BUILDDIR=build

LIBS = -lprotobuf-c -lzmq
DEPS = inputs.h message-commands.h serial-commands.h
OBJS = \
  $(BUILDDIR)/commands.pb-c.o \
  $(BUILDDIR)/inputs.o \
  $(BUILDDIR)/message-commands.o \
  $(BUILDDIR)/serial-commands.o \
  $(BUILDDIR)/tv-serial.o \

all: tv-serial tv-client.py 

clean:
	rm -rf $(BUILDDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(BUILDDIR)/%.o: %.c $(DEPS) $(BUILDDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

tv-serial: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(BUILDDIR)/commands.pb-c.c $(BUILDDIR)/commands.pb-c.h: commands.proto $(BUILDDIR)
	protoc-c --c_out=$(BUILDDIR) $<

tv-client.py: $(BUILDDIR)/commands_pb2.py

$(BUILDDIR)/commands_pb2.py: commands.proto $(BUILDDIR)
	touch $(BUILDDIR)/__init__.py
	protoc --python_out=$(BUILDDIR) $<
