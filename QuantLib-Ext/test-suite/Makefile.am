
QL_TESTS = \
	quantlibexttestsuite.cpp \
	speedlevel.hpp \
	bonds.hpp bonds.cpp \
	calibratedcurve.hpp calibratedcurve.cpp \
	swaps.hpp swaps.cpp \
	utilities.hpp utilities.cpp

dist-hook:
	mkdir -p $(distdir)/build
	mkdir -p $(distdir)/bin

if BOOST_UNIT_TEST_FOUND

AM_CPPFLAGS = -I${top_srcdir} -I${top_builddir} ${BOOST_UNIT_TEST_DEFINE}

noinst_LTLIBRARIES = libUnitMain.la
libUnitMain_la_SOURCES = main.cpp
libUnitMain_la_CXXFLAGS = ${BOOST_UNIT_TEST_MAIN_CXXFLAGS}

bin_PROGRAMS = quantlibext-test-suite

quantlibext_test_suite_SOURCES = ${QL_TESTS}
quantlibext_test_suite_LDADD = libUnitMain.la ${top_builddir}/qlext/libQuantLibExt.la \
                               ${top_builddir}/../QuantLib/ql/libQuantLib.la \
                               -l${BOOST_UNIT_TEST_LIB} ${BOOST_THREAD_LIB}  ${BOOST_INTERPROCESS_LIB}


TESTS = quantlibext-test-suite$(EXEEXT)
TESTS_ENVIRONMENT = BOOST_TEST_LOG_LEVEL=message

endif
