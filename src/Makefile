MAKEFLAGS += -r
.SUFFIXES: .c .o

CFLAGS ?= -O2 -fno-plt -pipe -flto=auto
CFLAGS += -Wall -Wvla -pthread
LDFLAGS ?= -Wl,-O1
LDLIBS += -lpthread

SRC := $(wildcard *.c)
OBJ := $(patsubst %.c, %.o, $(SRC))

PHONY := all clean

all: libthrdpool.so

libthrdpool.so: thrdpool.o blocking_queue.o
	$(LINK.o) -shared $^ $(LOADLIBES) $(LDLIBS) -o $@

thrdpool.o: thrdpool.c thrdpool.h blocking_queue.h cmacs.h
	$(COMPILE.c) -fPIC $< -o $@

blocking_queue.o: blocking_queue.c blocking_queue.h cmacs.h
	$(COMPILE.c) -fPIC $< -o $@

clean:
	$(RM) -- $(OBJ) libthrdpool.so

cmacs.h:
	$(RM) -- $@
	@wget -qc --show-progress -t 3 --waitretry=3 -O $@ 'https://raw.githubusercontent.com/ArcanusNEO/cmacs/master/cmacs.h'

.c.o:
	$(COMPILE.c) $< -o $@

.SECONDARY: $(OBJ)
.PHONY: $(PHONY)
