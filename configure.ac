AC_PREREQ(2.59)
AC_INIT(photoSigner, 0.3, cesar@crichard.fr)
AM_INIT_AUTOMAKE

AC_PROG_CC
AC_PROG_CXX
AC_PROG_MAKE_SET

AC_LANG([C++])

CPPFLAGS="$CPPFLAGS -I/usr/include/ImageMagick -I/home/cesar/igep/uav_dev_svn/tags/latest/include"
CFLAGS="$CFLAGS -Wall -O3"
LIBS="$LIBS -lMagick++ -lMagickCore -lexiv2"

AC_CONFIG_FILES([
        Makefile
        src/Makefile
        ])

AC_OUTPUT

