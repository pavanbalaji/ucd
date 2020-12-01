##
## Copyright (C) by Argonne National Laboratory
##     See COPYRIGHT in top-level directory
##

AC_PREREQ(2.63)

m4_include([maint/version.m4])
AC_INIT([ucd],UCD_VERSION_m4)

UCD_VERSION=UCD_VERSION_m4
AC_SUBST([UCD_VERSION])

libucd_so_version="libucd_so_version_m4"
AC_SUBST([libucd_so_version])

# Produce a numeric version assuming the following format:
# Version: [MAJ].[MIN].[REV][EXT][EXT_NUMBER]
# Example: 1.0.7rc1 has
#          MAJ = 1
#          MIN = 0
#          REV = 7
#          EXT = rc
#          EXT_NUMBER = 1
#
# Converting to numeric version will convert EXT to a format number:
#          ALPHA (a) = 0
#          BETA (b)  = 1
#          RC (rc)   = 2
#          PATCH (p) = 3
# Regular releases are treated as patch 0
#
# Numeric version will have 1 digit for MAJ, 2 digits for MIN,
# 2 digits for REV, 1 digit for EXT and 2 digits for EXT_NUMBER.
if test "${UCD_VERSION}" = "unreleased" ; then
    UCD_NUMVERSION=0
else
    changequote(<<,>>)
    V1=`expr $UCD_VERSION : '\([0-9]*\)\.[0-9]*\.*[0-9]*[a-zA-Z]*[0-9]*'`
    V2=`expr $UCD_VERSION : '[0-9]*\.\([0-9]*\)\.*[0-9]*[a-zA-Z]*[0-9]*'`
    V3=`expr $UCD_VERSION : '[0-9]*\.[0-9]*\.*\([0-9]*\)[a-zA-Z]*[0-9]*'`
    V4=`expr $UCD_VERSION : '[0-9]*\.[0-9]*\.*[0-9]*\([a-zA-Z]*\)[0-9]*'`
    V5=`expr $UCD_VERSION : '[0-9]*\.[0-9]*\.*[0-9]*[a-zA-Z]*\([0-9]*\)'`
    changequote([,])

    if test "$V2" -le 9 ; then V2="0$V2" ; fi
    if test "$V3" = "" ; then V3="0"; fi
    if test "$V3" -le 9 ; then V3="0$V3" ; fi
    if test "$V4" = "a" ; then
	V4=0
    elif test "$V4" = "b" ; then
	V4=1
    elif test "$V4" = "rc" ; then
	V4=2
    elif test "$V4" = "" ; then
	V4=3
	V5=0
    elif test "$V4" = "p" ; then
	V4=3
    fi
    if test "$V5" -le 9 ; then V5="0$V5" ; fi

    UCD_NUMVERSION=`expr $V1$V2$V3$V4$V5 + 0`
fi
AC_SUBST(UCD_NUMVERSION)

AC_CONFIG_AUX_DIR(m4)
AC_CONFIG_MACRO_DIR(m4)


dnl ----------------------------------------------------------------------------
dnl setup autotools
dnl ----------------------------------------------------------------------------
AM_INIT_AUTOMAKE([-Wall -Wno-portability-recursive -Werror foreign 1.12.3 subdir-objects])
AM_SILENT_RULES([yes])

PAC_PUSH_FLAG([CFLAGS])
AM_PROG_AS
PAC_POP_FLAG([CFLAGS])

AM_PROG_AR

LT_INIT

AC_PROG_INSTALL


dnl ----------------------------------------------------------------------------
dnl check the compiler and standard headers
dnl ----------------------------------------------------------------------------
dnl disable AC_PROG_CC's annoying default of adding -O2 to the CFLAGS
PAC_PUSH_FLAG([CFLAGS])
AC_PROG_CC
AC_PROG_CC_C99
PAC_POP_FLAG([CFLAGS])

AC_HEADER_STDC

# required pre-Automake-1.14
AM_PROG_CC_C_O

dnl ----------------------------------------------------------------------------
dnl configure options
dnl ----------------------------------------------------------------------------
# --enable-debug
AC_ARG_ENABLE([debug],AS_HELP_STRING([--enable-debug],[adds -g to CFLAGS]),,[enable_debug=no])
if test "${enable_debug}" != "no" ; then
    PAC_APPEND_FLAG([-g],[CFLAGS])
fi

# --enable-fast
AC_ARG_ENABLE([fast],AS_HELP_STRING([--enable-fast=O<opt>],[adds -O<opt> to CFLAGS]),,[enable_fast=O2])
PAC_PUSH_FLAG(IFS)
IFS=","
for option in ${enable_fast} ; do
    case "$option" in
        O*)
            PAC_APPEND_FLAG([-${option}],[CFLAGS])
	    ;;
	ndebug)
	    PAC_APPEND_FLAG([-DNDEBUG],[CFLAGS])
	    ;;
	*)
	    # ignore unknown options
	    ;;
    esac
done
PAC_POP_FLAG(IFS)

# --with-yaksa
PAC_SET_HEADER_LIB_PATH([yaksa])
PAC_PUSH_FLAG([LIBS])
PAC_CHECK_HEADER_LIB([yaksa.h],[yaksa],[yaksa_init],[have_yaksa=yes],[have_yaksa=no])
PAC_POP_FLAG([LIBS])
if test "${have_yaksa}" = "yes" ; then
    AC_DEFINE([HAVE_YAKSA],[1],[Define if Yaksa is available])
    AC_DEFINE_UNQUOTED([LIBYAKSA],["${with_yaksa}/lib/libyaksa.so"],[Define to Yaksa's path])
fi
AM_CONDITIONAL([HAVE_YAKSA], [test x${have_yaksa} = xyes])


dnl ----------------------------------------------------------------------------
dnl check the environment and the availability of functions
dnl ----------------------------------------------------------------------------
AC_CHECK_LIB([dl],[dlopen],[PAC_APPEND_FLAG([-ldl],[LIBS])],[AC_MSG_ERROR([dl library not found])])


dnl ----------------------------------------------------------------------------
dnl Final setup
dnl ----------------------------------------------------------------------------


dnl ----------------------------------------------------------------------------
dnl config headers
dnl ----------------------------------------------------------------------------
AC_CONFIG_HEADERS([src/ucdconfig.h])
AH_TOP([
/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#ifndef UCDCONFIG_H_INCLUDED
#define UCDCONFIG_H_INCLUDED
])
AH_BOTTOM([
#endif /* UCDCONFIG_H_INCLUDED */
])

AC_CONFIG_FILES([Makefile
                 Doxyfile
                 maint/ucd.pc
		 src/ucd.h
])
AC_OUTPUT


##################################################################
# Display the status of optional modules
##################################################################

cat <<EOF

##################################################################
# Final status of compiled ucd modules
##################################################################

##################################################################
# End of ucd configure
##################################################################

EOF
