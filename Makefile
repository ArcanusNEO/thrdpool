MAKEFLAGS += -r
.SUFFIXES: .c .o

CFLAGS += -Wall -Wvla -pthread
# LDLIBS += -lm

SRC := $(wildcard *.c)
OBJ := $(patsubst %.c, %.o, $(SRC))
EXE := libthrdpool.so

PHONY := all clean

all: $(EXE)

libthrdpool.so: $(OBJ) $(SRC) cmacs.h
	$(CC) -shared $(CFLAGS) -o$@ $(OBJ) $(LDLIBS)

clean:
	$(RM) -- $(OBJ) $(EXE) cmacs.h

.c.o:
	$(CC) -fPIC -c $(CFLAGS) $(CPPFLAGS) -o$@ $^

cmacs.h:
	$(RM) -- cmacs.h
	@wget -qc --show-progress -t 3 --waitretry=3 'https://raw.githubusercontent.com/ArcanusNEO/cmacs/master/cmacs.h'

.SECONDARY: $(OBJ)
.PHONY: $(PHONY)
