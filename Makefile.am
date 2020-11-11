ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -Wall
AM_CFLAGS 	=
AM_CXXFLAGS =
AM_LDFLAGS 	= -ldl

lib_LTLIBRARIES =

lib_LTLIBRARIES += libfoo1.la
libfoo1_la_SOURCES = src/impl1/foo.c

lib_LTLIBRARIES += libfoo2.la
libfoo2_la_SOURCES = src/impl2/foo.c

check_PROGRAMS =

check_PROGRAMS += test1
test1_SOURCES = tests/test1.cc

TESTS = $(check_PROGRAMS)
