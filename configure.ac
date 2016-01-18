AC_INIT([GNU Rocket], [0.9.0-master], [kusmabite@gmail.com])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([foreign])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_LIBTOOL
AC_PROG_CC
AC_PROG_CXX

PKG_CHECK_MODULES(SDL, sdl)
PKG_CHECK_MODULES(GL, gl)
PKG_CHECK_MODULES(GLU, glu)

AC_PATH_PROGS(QMAKE, [qmake qmake-qt4], "no", [$PATH:/opt/local/libexec/qt5/bin])
AS_IF([test "x$QMAKE" = "xno"], [AC_MSG_ERROR([Unable to find Qt])])

AC_ARG_WITH([bass],
            [AS_HELP_STRING([--with-bass=DIR],
                            [Use Un4Seen Developments BASS library from <DIR>])],
            [BASS_CPPFLAGS="-I$withval"
             BASS_LDFLAGS="-L$withval -L$withval/x64"],
            [BASS_CPPFLAGS=""
             BASS_LDFLAGS="-lbass"])

AS_IF([test "x$with_bass" != xno],
      [AX_CHECK_LIBRARY([BASS], [bass.h], [bass],
                        [have_bass=yes],
                        [AS_IF([test "x$with_bass" != x],
                               [AC_MSG_ERROR([Unable to find bass])])])])

AM_CONDITIONAL([HAVE_BASS], [test "x$have_bass" = xyes])
AC_MSG_NOTICE([have_bass: $have_bass])

AC_CONFIG_FILES([
	Makefile
	lib/Makefile
	editor/Makefile
	examples/Makefile
])
AC_OUTPUT
