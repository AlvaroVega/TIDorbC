AC_DEFUN([TID_CHECK_COMPRESSORS],
  [

    AC_ARG_WITH([zlib],
      AS_HELP_STRING([--with-zlib@<:@=DIR@:>@],
                     [root directory of Zlib installation]),
      with_zlib="${withval}")


    AC_MSG_CHECKING([for Zlib])
    
    for dir in $with_ssl /usr/pkg /usr/local /usr; do
        zlibdir="$dir"
	if test -f "$dir/include/zlib.h"; then
	   found_zlib="yes";
	   ZLIB_CXXFLAGS="-I$zlibdir/include";
	   break
	fi
    done

    if test x_$found_zlib != x_yes; then
	AC_MSG_ERROR([Cannot locate Zlib header])
    else
	AC_MSG_RESULT([$zlibdir])
        if test "$zlibdir" == "$with_zlib"; then
           ZLIB_LIBS="-L$zlibdir/lib -lz";
        else
           ZLIB_LIBS="-lz";
        fi
    fi


    dnl Check for COMPRESSORS pthread linking
    saved_CXXFLAGS="$CXXFLAGS"
    saved_LDFLAGS="$LDFLAGS"

    dnl LDFLAGS="$LDFLAGS -lz"
    CXXFLAGS="$CXXFLAGS $ZLIB_CXXFLAGS"
    LDFLAGS="$LDFLAGS $ZLIB_LIBS"

    AC_MSG_CHECKING([COMPRESSORS linking])
    AC_LINK_IFELSE(
      AC_LANG_PROGRAM(
        [[
          #include <zlib.h>

          void foobar() {}
        ]],
        [[
          int ret;
          z_stream strm;
          strm.zalloc = Z_NULL;
          strm.zfree = Z_NULL;
          strm.opaque = Z_NULL;
          strm.avail_in = 0;
          strm.next_in = Z_NULL;
          ret = inflateInit(&strm);

        ]]
      ),
      [AC_MSG_RESULT([yes])],
      [AC_MSG_FAILURE([cannot link with compressors library])]
    )

    dnl Set variables
    AC_SUBST([COMPRESSORS_LIB], ["$ZLIB_LIBS"])
    LDFLAGS="$saved_LDFLAGS"
    dnl CXXFLAGS="$saved_CXXFLAGS"

  ]
)

