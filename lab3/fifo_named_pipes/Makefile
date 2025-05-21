# Compiler und Flags
CC      := gcc
CFLAGS  := -Wall -Wextra -O2

# Alle .c-Quellen und daraus die Ziel-Programme
SRCS    := $(wildcard *.c)
PROGS   := $(SRCS:.c=)

.PHONY: all clean

# Default-Ziel: baue alle Programme
all: $(PROGS)

# Regel: foo ← foo.c
# kompiliert foo.c direkt zu ./foo
$(PROGS): %: %.c
	$(CC) $(CFLAGS) $< -o $@

# Aufräumen: lösche alle erzeugten Programme
clean:
	rm -f $(PROGS)
