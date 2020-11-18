ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -Wall -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/win32"
AM_CFLAGS 	=
AM_CXXFLAGS =
AM_LDFLAGS 	= -ldl

lib_LTLIBRARIES =

lib_LTLIBRARIES += libfoo1.la
libfoo1_la_SOURCES = src/impl1/foo.c

lib_LTLIBRARIES += libfoo2.la
libfoo2_la_SOURCES = src/impl2/foo.c

lib_LTLIBRARIES += libdynamic-library.la
libdynamic_library_la_SOURCES = src/dynamic_library.cpp

check_PROGRAMS =

check_PROGRAMS += test1
test1_SOURCES = tests/test1.cc
test1_LDADD = libdynamic-library.la

check_PROGRAMS += test-dlopen-jvm
test_dlopen_jvm_SOURCES = tests/test-dlopen-jvm.cc
test_dlopen_jvm_LDADD = libdynamic-library.la

TESTS = $(check_PROGRAMS)
