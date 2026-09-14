# Public SDK example build (host gcc; also used by CI).
CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra

all: v4l2_capture

v4l2_capture: v4l2_capture.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f v4l2_capture
