# Makefile for building application targeting QEMU system
# by Brian Fraser

# Edit this file to compile extra C files into their own programs.
TARGET=array_stats_test process_ancestors_test

CC_C = gcc

CFLAGS = -Werror -Wall -g -std=c99 -D _GNU_SOURCE -static -pthread

all: clean $(TARGET)

$(TARGET):
	$(CC_C) $(CFLAGS) $@.c -o $@

clean:
	rm -f $(TARGET)

transfer:
	scp -P 10022 $(TARGET) root@localhost:~

killqemu:
	killall qemu-system-x86_64

