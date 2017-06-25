
AM_CPPFLAGS = -I${top_srcdir} -I${top_builddir}

SUBDIRS = \
    CTBZeroBond

SUBDIR_CHECKS = $(SUBDIRS:%=%.check)

EXTRA_DIST = CMakeLists.txt

%.check:
	$(MAKE) -C $* check-examples

.PHONY: examples check-examples $(SUBDIRS)

examples: $(SUBDIRS)

check-examples: $(SUBDIR_CHECKS)

$(SUBDIRS):
	$(MAKE) -C $@ examples
