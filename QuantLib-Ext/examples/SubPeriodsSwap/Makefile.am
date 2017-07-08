
AM_CPPFLAGS = -I${top_srcdir} -I${top_builddir}

if AUTO_EXAMPLES
bin_PROGRAMS = SubPeriodsSwap
TESTS = SubPeriodsSwap$(EXEEXT)
else
noinst_PROGRAMS = SubPeriodsSwap
endif
SubPeriodsSwap_SOURCES = SubPeriodsSwap.cpp
SubPeriodsSwap_LDADD = ../../qlext/libQuantLibExt.la  ../../../QuantLib/ql/libQuantLib.la ${BOOST_THREAD_LIB}

.PHONY: examples check-examples

examples: SubPeriodsSwap$(EXEEXT)

check-examples: examples
	./SubPeriodsSwap$(EXEEXT)

dist-hook:
	mkdir -p $(distdir)/bin
	mkdir -p $(distdir)/build
