
AM_CPPFLAGS = -I${top_srcdir} -I${top_builddir}

if AUTO_EXAMPLES
bin_PROGRAMS = CTBZeroBond
TESTS = CTBZeroBond$(EXEEXT)
else
noinst_PROGRAMS = CTBZeroBond
endif
CTBZeroBond_SOURCES = CTBZeroBond.cpp
CTBZeroBond_LDADD = ../../qlext/libQuantLibExt.la  ../../../QuantLib/ql/libQuantLib.la ${BOOST_THREAD_LIB}

.PHONY: examples check-examples

examples: CTBZeroBond$(EXEEXT)

check-examples: examples
	./CTBZeroBond$(EXEEXT)

dist-hook:
	mkdir -p $(distdir)/bin
	mkdir -p $(distdir)/build
