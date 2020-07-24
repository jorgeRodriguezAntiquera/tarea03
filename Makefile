SRCDIR=src

default: all

all:
	@cd $(SRCDIR) && $(MAKE)

clean:
	@cd $(SRCDIR) && $(MAKE) clean
	@rm -rf *~ core

distclean: clean
	@cd $(SRCDIR) && $(MAKE) distclean

.PHONY: all clean distclean
