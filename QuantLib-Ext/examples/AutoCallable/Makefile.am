
AM_CPPFLAGS = -I${top_srcdir} -I${top_builddir}

if AUTO_EXAMPLES
bin_PROGRAMS = AutoCallable
TESTS = AutoCallable$(EXEEXT)
else
noinst_PROGRAMS = AutoCallable
endif
AutoCallable_SOURCES = AutoCallable.cpp
AutoCallable_LDADD = ../../qlext/libQuantLibExt.la  ../../../QuantLib/ql/libQuantLib.la ${BOOST_THREAD_LIB}

.PHONY: examples check-examples

examples: AutoCallable$(EXEEXT)

check-examples: examples
	./AutoCallable$(EXEEXT)

dist-hook:
	mkdir -p $(distdir)/bin
	mkdir -p $(distdir)/build
