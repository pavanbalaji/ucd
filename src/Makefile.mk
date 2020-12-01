##
## Copyright (C) by Argonne National Laboratory
##     See COPYRIGHT in top-level directory
##

if HAVE_YAKSA
libucd_la_SOURCES += src/ucd_wrappers_yaksa.c
else
libucd_la_SOURCES += src/ucd_wrappers_stub.c
endif !HAVE_YAKSA

libucd_la_SOURCES += src/ucdi_init.c

noinst_HEADERS += src/ucd_internal.h

include_HEADERS += src/ucd.h
