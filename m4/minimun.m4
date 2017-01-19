AC_DEFUN([TID_CHECK_MINIMUN],
  [
    AC_MSG_CHECKING([wheter minimun is enabled])

    AC_ARG_ENABLE([minimun],
      [  --enable-minimun=[yes/no] Turn on minimun],
      minimun=false 
      [case "${enableval}" in
             yes) minimun=true 
                  AC_MSG_RESULT([yes]) ;;
             no)  minimun=false 
                  AC_MSG_RESULT([no]) ;;
             *) AC_MSG_ERROR([bad value ${enableval} for --enable-minimun]) ;;
       esac],[minimun=false])
    AM_CONDITIONAL([MINIMUN], [test x$minimun = xtrue])
    if test "x$minimun" = xtrue; then
       AC_DEFINE(MINIMUN, 1, [Enabled minimun CORBA])
#    else
#       AC_DEFINE(MINIMUN, 0, [Disabled minimun CORBA])
    fi

  ]
)

