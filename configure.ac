#                                              -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([linuxlearning], [0.0.1], [zgxcassar@gmail.com])
#AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE
AC_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

LT_INIT
# Checks for libraries.
# FIXME: Replace `main' with a function in `-lgtest':
AC_CHECK_LIB([gtest], [main])
# FIXME: Replace `main' with a function in `-lll':
AC_CHECK_LIB([ll], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([rt], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h stddef.h stdint.h stdlib.h string.h strings.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([dup2 gettimeofday localtime_r memset rmdir strcasecmp strchr strdup strerror])

AX_HAVE_EPOLL(AX_CONFIG_FEATURE_ENABLE(epoll), AX_CONFIG_FEATURE_DISABLE(epoll))

AX_CONFIG_FEATURE_DEFAULT_ENABLED
AX_CONFIG_FEATURE(epoll, [turn on epoll support], HAVE_EPOLL, [define HAVE_EPOLL])

AC_ARG_ENABLE(colorlog, AC_HELP_STRING([--enable-colorlog=@<:@yes/no@:>@"],["enable colored log"])
                        [], 
                        [enable_color_log=no])

AS_IF([test "x$enable_color_log" != xno], [AC_DEFINE([COLOR_LOG])], [])


AC_CONFIG_FILES([Makefile
                 common/Makefile
                 test/Makefile])
AC_OUTPUT
