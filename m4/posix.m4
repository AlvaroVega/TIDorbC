AC_DEFUN([TID_CHECK_POSIX],
  [
    dnl Check for POSIX headers
    AC_CHECK_HEADERS(
      [pthread.h time.h sys/time.h signal.h unistd.h errno.h sched.h sys/types.h],
      [],
      [AC_MSG_ERROR([some POSIX headers not found])]
    )

    dnl Check for POSIX pthread linking
    saved_LDFLAGS="$LDFLAGS"
    LDFLAGS="$LDFLAGS -lpthread"
    AC_MSG_CHECKING([POSIX pthread linking])
    AC_LINK_IFELSE(
      AC_LANG_PROGRAM(
        [[
          #include <pthread.h>

          void foobar() {}
        ]],
        [[
          pthread_t thr;
          pthread_create(&thr, NULL, foobar, NULL);
          pthread_exit(NULL);          
        ]]
      ),
      [AC_MSG_RESULT([yes])],
      [AC_MSG_FAILURE([cannot link to pthread library])]
    )

    dnl Set variables
    AC_SUBST([PTHREAD_LIB], ["-lpthread"])
    LDFLAGS="$saved_LDFLAGS"

  ]
)

