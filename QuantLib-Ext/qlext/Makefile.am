SUBDIRS = cashflows indexes instruments termstructures

AM_CPPFLAGS = -I${top_srcdir} -I${top_builddir}

this_includedir = ${includedir}/${subdir}

this_include_HEADERS = \
	config.hpp \
	auto_link.hpp \
	qlextdefines.hpp \
	quantlibext.hpp

libQuantLibExt_la_SOURCES =

lib_LTLIBRARIES = libQuantLibExt.la
libQuantLibExt_la_LDFLAGS = -version-info 0:0:0

libQuantLibExt_la_LIBADD = \
    cashflows/libCashflows.la \
	indexes/libIndexes.la \
    instruments/libInstruments.la \
	termstructures/libTermStructures.la
