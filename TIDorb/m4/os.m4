AC_DEFUN([TID_CHECK_OS],
  [
    AC_MSG_CHECKING([for operating system family])

    OS_FAMILY="`uname`"
    case "${OS_FAMILY}" in
		SunOS)
        AC_DEFINE([__sunos__])
        AC_DEFINE([__sunos])
        AC_DEFINE([__sun__])
        AC_DEFINE([__sun])
        AC_MSG_RESULT([SunOS])
        ;;
      Linux)
        AC_DEFINE([__linux__])
        AC_DEFINE([__linux])
        AC_MSG_RESULT([Linux])
        ;;
      Darwin)
        AC_DEFINE([__darwin__])
        AC_DEFINE([__darwin])
        AC_MSG_RESULT([Darwin])
        ;;
      *)
        AC_MSG_RESULT([Unknown])
        ;;
    esac
  ]
)

