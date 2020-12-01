##
## Copyright (C) by Argonne National Laboratory
##     See COPYRIGHT in top-level directory
##

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = autogen.sh README.md
EXTRA_PROGRAMS =

.PHONY: doxygen

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = maint/ucd.pc

noinst_HEADERS =
include_HEADERS =
noinst_PROGRAMS =
nodist_noinst_SCRIPTS =
dist_noinst_SCRIPTS = autogen.sh

lib_LTLIBRARIES = libucd.la

libucd_la_SOURCES =
AM_CPPFLAGS =

libucd_la_LDFLAGS = -version-info @libucd_so_version@

include $(top_srcdir)/src/Makefile.mk
include $(top_srcdir)/examples/Makefile.mk

examples: $(example_list)

doxygen:
	mkdir -p doc
	doxygen Doxyfile
