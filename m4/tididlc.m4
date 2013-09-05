AC_DEFUN([TID_CHECK_TIDIDLC],
  [

    AC_REQUIRE([TID_CHECK_JRE])

    dnl Look for TID IDL compiler installation directory
    AC_MSG_CHECKING([for TID IDL compiler path])
    AC_ARG_WITH(
      [tididlc],
      AS_HELP_STRING([--with-tididlc@<:@=DIR@:>@],
                     [prefix for TID IDL compiler install path]),
dnl      [  --with-tididlc=DIR  prefix for TID IDL compiler install path],
      [
        if test "$withval" = "yes"; then
          tididlc_prefix=/usr
          export TIDIDLC_HOME=${tididlc_prefix}
          AC_MSG_RESULT([using default prefix])
        elif test "$withval" = "no"; then
          AC_MSG_ERROR(
            TID IDL compiler is needed, cannot be avoid using argument --with-tididlc=no)
        else
          tididlc_prefix="$withval"
          export TIDIDLC_HOME=${tididlc_prefix}
          AC_MSG_RESULT([$tididlc_prefix])
        fi
      ],
      [
        if test -z ${TIDIDLC_HOME} ; then
          tididlc_prefix="/usr"
          export TIDIDLC_HOME=${tididlc_prefix}
          AC_MSG_RESULT([using default prefix $tididlc_prefix])
        else
          AC_MSG_RESULT([using variable TIDIDLC_HOME, $TIDIDLC_HOME])
          tididlc_prefix="$TIDIDLC_HOME"
        fi
      ]
    )

    AC_MSG_CHECKING([for idl2cpp.jar library])
    if test -f ${tididlc_prefix}/lib/idl2cpp.jar ; then
       AC_MSG_RESULT([found])
    else
       AC_MSG_FAILURE([not found])
    fi

    AC_MSG_CHECKING([for TID IDL compiler to execute])

    abs_srcdir="`cd $srcdir && pwd`"
    tididlc_bin="${abs_srcdir}/idlc-wrapper.sh"
    if test -x ${tididlc_bin}; then
      AC_MSG_RESULT([yes])
      AC_SUBST([IDLC], ["eval ${tididlc_bin} --java-bin=$JAVA_HOME/bin/java --tididlc-home=$tididlc_prefix"])
    else
      AC_MSG_FAILURE([cannot be executed ${tididlc_bin}])
    fi

    dnl Check for IDL compiler correct version
    if test -n "$1"; then
      AC_MSG_CHECKING([TID IDL compiler version])

      idlc_version=`${IDLC} --version | grep version | cut -d" " -f5`
      idlc_version1=`echo $idlc_version | cut -d"." -f1,2`
      idlc_version2=`echo $idlc_version | cut -d"." -f3`

      idlc_needed_version=$1
      idlc_needed_version1=`echo $idlc_needed_version | cut -d"." -f1,2`
      idlc_needed_version2=`echo $idlc_needed_version | cut -d"." -f3`
      idlc_invalid_version_msg="version ${idlc_needed_version} or greater needed"

      if test x"`echo "$idlc_version1 < $idlc_needed_version1" | bc`" = x"1"; then
        AC_MSG_FAILURE([${idlc_invalid_version_msg}])
      elif test x"$idlc_version1" = x"$idlc_needed_version1"; then
        if test x"`echo "$idlc_version2 < $idlc_needed_version2" | bc`" = x"1"; then
          AC_MSG_FAILURE([${idlc_invalid_version_msg}])
        fi
      fi

      AC_MSG_RESULT([${idlc_version}])
    fi

    AC_MSG_CHECKING([wheter IDL compiler works])
    foo_idl=".foo_idl"
    foo_idl_body="module foo_module { interface foo_interface{}; };"
    echo "${foo_idl_body}" > ${foo_idl}.idl
    if ${IDLC} ${foo_idl}.idl; then
      rm ${foo_idl}.h ${foo_idl}.C POA_${foo_idl}.h POA_${foo_idl}.C
      AC_MSG_RESULT([yes])
    else
      AC_MSG_FAILURE([IDL compiler fails])
    fi
    rm ${foo_idl}.idl 

  ]
)

