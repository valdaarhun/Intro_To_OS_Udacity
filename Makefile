export CC := gcc
export CFLAGS := -Wall
export LDFLAGS := -pthread

all: readers_writers echo_server

readers_writers:
	$(MAKE) -C priority_readers_writers

echo_server:
	$(MAKE) -C fictitious_echo_server