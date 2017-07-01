#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(QuantLibExt, 1.0.0, wegamekinglc@hotmail.com, QuantLibExt)
AC_CONFIG_SRCDIR(qlext/qlextdefines.hpp)
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS(qlext/config.hpp)
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_FILES([
    Makefile
    qlext/Makefile
    qlext/instruments/Makefile
    qlext/instruments/bonds/Makefile
    qlext/termstructures/Makefile
    qlext/termstructures/yield/Makefile
    qlext/time/Makefile
    qlext/time/daycounters/Makefile
    config/Makefile
    m4/Makefile
    examples/Makefile
    examples/CTBZeroBond/Makefile
    test-suite/Makefile])

# Checks for programs.
AC_PROG_AWK
AC_PROG_INSTALL

AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_LIBTOOL
AC_LANG([C++])

AC_MSG_CHECKING([whether to install examples])
AC_ARG_ENABLE([examples],
              AC_HELP_STRING([--enable-examples],
                             [If enabled, examples are built and installed
                              when "make" and "make install" are invoked.
                              If disabled (the default) they are built
                              but not installed.]),
              [ql_install_examples=$enableval],
              [ql_install_examples=no])
AM_CONDITIONAL(AUTO_EXAMPLES, test "$ql_install_examples" != "no")
AC_MSG_RESULT([$ql_install_examples])

QL_CHECK_BOOST
AM_CONDITIONAL(BOOST_UNIT_TEST_FOUND, test "x${BOOST_UNIT_TEST_LIB}" != "x")

AC_OUTPUT
