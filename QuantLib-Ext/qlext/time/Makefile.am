
SUBDIRS = daycounters

AM_CPPFLAGS = -I${top_srcdir} -I${top_builddir}

this_includedir=${includedir}/${subdir}
this_include_HEADERS = 

cpp_files = 

libTime_la_SOURCES = $(cpp_files)

libTime_la_LIBADD = daycounters/libDayCounters.la

noinst_LTLIBRARIES = libTime.la