CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2 -I include
LDFLAGS ?=

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

SRCS_COMMON = src/output.c src/xbps-wrapper.c
OBJS_COMMON = $(SRCS_COMMON:src/%.c=build/%.o)

TARGETS = build/xpt build/xman

all: $(TARGETS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/xpt: src/xpt.c $(OBJS_COMMON)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

build/xman: src/xman.c $(OBJS_COMMON)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

install: all
	install -Dm755 build/xpt $(DESTDIR)$(BINDIR)/xpt
	install -Dm755 build/xman $(DESTDIR)$(BINDIR)/xman

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/xpt
	rm -f $(DESTDIR)$(BINDIR)/xman

clean:
	rm -rf build

.PHONY: all install uninstall clean
