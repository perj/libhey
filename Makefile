
CFLAGS = -g -Os -Wall
LDFLAGS = -g

all: test_lookup test_connect

test_lookup: test_lookup.o lookup.o

test_connect: test_connect.o lookup.o connect.o poller.o
