
AM_CPPFLAGS = -I${top_srcdir} -I${top_builddir}

this_includedir=${includedir}/${subdir}
this_include_HEADERS = \
    ctbfixedbond.hpp \
    ctbzerobond.hpp

cpp_files = \
    ctbfixedbond.cpp \
    ctbzerobond.cpp


libBonds_la_SOURCES = $(cpp_files)

noinst_LTLIBRARIES = libBonds.la
