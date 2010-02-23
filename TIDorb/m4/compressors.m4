AC_DEFUN([TID_CHECK_COMPRESSORS],
  [
    dnl Check for COMPRESSORS headers
    AC_CHECK_HEADERS(
      [zlib.h],
      [],
      [AC_MSG_ERROR([some COMPRESSORS headers not found])]
    )

    dnl Check for COMPRESSORS pthread linking
    saved_LDFLAGS="$LDFLAGS"
    LDFLAGS="$LDFLAGS -lz"
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
    AC_SUBST([COMPRESSORS_LIB], ["-lz"])
    LDFLAGS="$saved_LDFLAGS"

  ]
)

