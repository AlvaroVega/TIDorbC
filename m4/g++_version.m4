AC_DEFUN([TID_CPP_VERSION],
  [
    dnl Requires AC_PROG_CXX
    AC_REQUIRE([AC_PROG_CXX])

    dnl Check if GNU C++ compiler is in use
    AC_MSG_CHECKING([if using GNU C++ compiler])
    if test x"${GCC}" = x"yes"; then
      AC_MSG_RESULT([yes])

      dnl Check for JAVA_HOME variable
      AC_MSG_CHECKING([for GNU C++ compiler version])
      cxx_version=`${CXX} --version 2>&1 | head -n 1 | cut -d' ' -f3`
      cxx_major_version=`echo $cxx_version | cut -d"." -f1,2`
      cxx_minor_version=`echo $cxx_version | cut -d"." -f3`
  
      cxx_needed_version=$1
      cxx_needed_major_version=`echo $cxx_needed_version | cut -d"." -f1,2`
      cxx_needed_minor_version=`echo $cxx_needed_version | cut -d"." -f3`
      cxx_invalid_version_msg="GNU C++ compiler version must be at least ${cxx_needed_version}"
  
      if test x"`echo "${cxx_major_version} < ${cxx_needed_major_version}" | bc`" = x"1"; then
        AC_MSG_FAILURE([${cxx_invalid_version_msg}])
      elif test x"$cxx_major_version" = x"$cxx_needed_major_version"; then
        if test x"`echo "${cxx_minor_version} < ${cxx_needed_minor_version}" | bc`" = x"1"; then
          AC_MSG_FAILURE([${cxx_invalid_version_msg}])
        fi
      fi
  
      AC_MSG_RESULT([${cxx_version}])
    else
      AC_MSG_RESULT([no])
    fi
  ]
)
