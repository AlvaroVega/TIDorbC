AC_DEFUN([TID_CHECK_JRE],
  [

    AC_REQUIRE([TID_CHECK_OS])

    dnl Check for JAVA_HOME variable
    AC_MSG_CHECKING([for JAVA_HOME variable])
    if test ${JAVA_HOME} ; then
      AC_MSG_RESULT([yes])
    else
      AC_MSG_RESULT([no])
      AC_MSG_CHECKING([for JDK_HOME variable])
      if test ${JDK_HOME}; then
        export JAVA_HOME=${JDK_HOME}
        AC_MSG_RESULT([yes])
      else
        AC_MSG_RESULT([no])
        AC_MSG_CHECKING([for JRE home directory in platform-dependent location])
        dnl Check for platform-dependent JRE locations
        case ${OS_FAMILY} in
          Darwin)
            export JAVA_HOME="/usr"
            AC_MSG_RESULT([using MacOSX JRE])
            ;;
          SunOS)
            export JAVA_HOME="/usr/java/jre"
            AC_MSG_RESULT([using SunOS JRE])
            ;;
          *)
            AC_MSG_FAILURE([unknown platform])
            ;;
        esac
      fi
    fi

    dnl Check for Java Runtime Machine binary
    AC_MSG_CHECKING([for JRE binary])
    if test -x ${JAVA_HOME}/bin/java; then
      JAVA="${JAVA_HOME}/bin/java"
      AC_MSG_RESULT([${JAVA}])
    else
      AC_MSG_FAILURE([not found])
    fi
  ]
)

