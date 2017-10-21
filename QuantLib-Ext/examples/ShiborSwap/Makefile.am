
AM_CPPFLAGS = -I${top_srcdir} -I${top_builddir}

if AUTO_EXAMPLES
bin_PROGRAMS = ShiborSwap
TESTS = ShiborSwap$(EXEEXT)
else
noinst_PROGRAMS = ShiborSwap
endif
ShiborSwap_SOURCES = ShiborSwap.cpp
ShiborSwap_LDADD = ../../qlext/libQuantLibExt.la  ../../../QuantLib/ql/libQuantLib.la ${BOOST_THREAD_LIB}

.PHONY: examples check-examples

examples: ShiborSwap$(EXEEXT)

check-examples: examples
	./ShiborSwap$(EXEEXT)

dist-hook:
	mkdir -p $(distdir)/bin
	mkdir -p $(distdir)/build