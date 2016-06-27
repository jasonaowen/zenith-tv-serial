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

$(BUILDDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tv-serial: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(BUILDDIR)/commands.pb-c.c $(BUILDDIR)/commands.pb-c.h: commands.proto
	protoc-c --c_out=$(BUILDDIR) $<
