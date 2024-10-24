SRC = $(wildcard *.c)
BIN = main
LDLIBS := -lseg
LDFLAGS := -L.
CFLAGS += -Wall -Wextra -g
#CFLAGS += --std=c99

all: $(BIN)

main.o: main.c segdef.h

clean:
	rm -f $(BIN)
	rm -f *.o
	rm -f *~

indent:
	indent -linux -i4 -nut -ts2 *.c

.PHONY: all clean indent	