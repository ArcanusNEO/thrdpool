MAKEFLAGS += -r
.PHONY: all clean install uninstall

prefix ?= /usr/local
exec_prefix := $(prefix)
bindir := $(exec_prefix)/bin
sbindir := $(exec_prefix)/sbin
libexecdir := $(exec_prefix)/libexec
datarootdir := $(prefix)/share
datadir := $(datarootdir)
sysconfdir := $(prefix)/etc
sharedstatedir := $(prefix)/com
localstatedir := $(prefix)/var
runstatedir := $(localstatedir)/run
includedir := $(prefix)/include
docdir := $(datarootdir)/doc
infodir := $(datarootdir)/info
htmldir := $(docdir)
dvidir := $(docdir)
pdfdir := $(docdir)
psdir := $(docdir)
libdir := $(exec_prefix)/lib
lispdir := $(datarootdir)/emacs/site-lisp
localedir := $(datarootdir)/locale
mandir := $(datarootdir)/man
manext := .1
srcdir := src

all:
	$(MAKE) -C $(srcdir) -- all

clean:
	$(MAKE) -C $(srcdir) -- clean
	$(RM) -- $(srcdir)/cmacs.h

install: all
	mkdir -p -- $(DESTDIR)$(includedir)
	install -m644 -- src/blocking_deque.h $(DESTDIR)$(includedir)
	install -m644 -- src/thrdpool.h $(DESTDIR)$(includedir)

	mkdir -p -- $(DESTDIR)$(libdir)
	install -m755 -- src/libthrdpool.so $(DESTDIR)$(libdir)

uninstall:
	$(RM) -- $(DESTDIR)$(libdir)/libthrdpool.so
	$(RM) -- $(DESTDIR)$(includedir)/thrdpool.h
	$(RM) -- $(DESTDIR)$(includedir)/blocking_deque.h
