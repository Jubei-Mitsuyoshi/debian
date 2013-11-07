#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.50])
AC_INIT([pup-volume-monitor],[0.1.15],[BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([libpupvm/pupvm-plugin.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign silent-rules])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AC_CONFIG_MACRO_DIR([m4])
AM_MAINTAINER_MODE
LT_INIT

# Checks for programs.
AC_PROG_CC
AC_SEARCH_LIBS([strerror],[cposix])

#AM_PROG_CC_C_O

# Checks for libraries.
AC_CHECK_LIB([m], [main])
PKG_CHECK_MODULES([BLKID], [blkid])
PKG_CHECK_MODULES([UDEV], [libudev])
PKG_CHECK_MODULES([GIO], [gio-2.0 >= 2.17.0 gthread-2.0 >= 2.17.0 gmodule-2.0 >= 2.17.0])

# cdrom plugin
AC_ARG_ENABLE(cdrom, AS_HELP_STRING([--disable-cdrom],[build without optical drive support]))
AM_CONDITIONAL(USE_CDROM, [test "x$enable_cdrom" != "xno"])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([floor select socket])


AC_CONFIG_FILES([Makefile
                 libpupvm/Makefile
                 data/pupvm.pc
                 tester/Makefile
                 daemon/Makefile
                 admin/Makefile
                 module/Makefile
                 plugins/Makefile
                 data/Makefile])
AC_OUTPUT


if test x"$sysconfdir" != x'/etc'; then
    echo
    echo
    echo 'Warning: sysconfdir is not /etc.'
    echo 'Please consider passing --sysconfdir=/etc to configure.';
    echo 'Otherwise default config files can be installed to wrong place.'
    echo
    echo
fi


