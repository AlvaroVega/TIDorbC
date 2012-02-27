AC_DEFUN([TID_CHECK_DEBUG],
  [
    AC_MSG_CHECKING([wheter debug is enabled])

    AC_ARG_ENABLE(debug, 
                  [  --enable-debug=[yes/no]   Turn on debugging ],
                  [
                    dnl Action if enabled 
                    CFLAGS="-g -O2"
                    CXXFLAGS="$CXXFLAGS -g -O2"
                    AC_MSG_RESULT([yes])
                  ],
                  [
                    dnl Action if disabled
                    CFLAGS="-O2"
                    CXXFLAGS="-O2"
                    AC_MSG_RESULT([no])
                  ])
  ]
)

