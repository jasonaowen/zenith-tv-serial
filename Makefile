CC=gcc
BUILDDIR=build

DEPS = commands.h inputs.h
OBJS = \
  $(BUILDDIR)/commands.o \
  $(BUILDDIR)/inputs.o \
  $(BUILDDIR)/tv-serial.o \

$(BUILDDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tv-serial: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
