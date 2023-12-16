# -*- Makefile -*- for ankerl/unordered_dense

.SECONDEXPANSION:
.SUFFIXES:

ifneq ($(findstring $(MAKEFLAGS),s),s)
ifndef V
        QUIET          = @
        QUIET_INSTALL  = @echo '   ' INSTALL $<;
        export V
endif
endif

RM       ?= rm -f

prefix ?= /usr/local
includedir := $(prefix)/include

HEADERS := $(wildcard include/ankerl/*h)

HEADERS_INST := $(patsubst include/%,$(includedir)/%,$(HEADERS))

.PHONY: install

all:

$(includedir)/%.h: include/%.h | $$(@D)/.
	$(QUIET_INSTALL)cp $< $@
	@chmod 0644 $@

install: $(HEADERS_INST)

clean:

distclean:

$(includedir)/.:
	$(QUIET)mkdir -p $@

$(includedir)%/.:
	$(QUIET)mkdir -p $@
