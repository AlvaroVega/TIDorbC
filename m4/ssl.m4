AC_DEFUN([TID_CHECK_SSL],
  [

    AC_ARG_WITH([openssl],
      AS_HELP_STRING([--with-openssl@<:@=DIR@:>@],
                     [root directory of OpenSSL installation]),
      with_ssl="${withval}")

    AC_MSG_CHECKING([for OpenSSL])
    
    for dir in $with_ssl /usr/local/ssl /usr/lib/ssl /usr/ssl /usr/pkg /usr/local /usr; do
        ssldir="$dir"
        if test -f "$dir/include/openssl/ssl.h"; then
           found_ssl="yes";
	   OPENSSL_CXXFLAGS="-I$ssldir/include";
	   break;
	fi
	if test -f "$dir/include/ssl.h"; then
	   found_ssl="yes";
	   OPENSSL_CXXFLAGS="-I$ssldir/include";
	   break
	fi
    done

    if test x_$found_ssl != x_yes; then
	AC_MSG_ERROR([Cannot locate SSL headers])
    else
	AC_MSG_RESULT([$ssldir])
        if test "$ssldir" == "$with_ssl"; then
           OPENSSL_LIBS="-L$ssldir/lib -lssl -lcrypto";
        else
           OPENSSL_LIBS="-lssl -lcrypto";
        fi
    fi
	

    dnl Check for SSL pthread linking
    saved_CXXFLAGS="$CXXFLAGS"
    saved_LDFLAGS="$LDFLAGS"

    dnl LDFLAGS="$LDFLAGS -lssl -lcrypto"
    CXXFLAGS="$CXXFLAGS $OPENSSL_CXXFLAGS"
    LDFLAGS="$LDFLAGS $OPENSSL_LIBS"

    AC_MSG_CHECKING([SSL linking])
    AC_LINK_IFELSE(
      AC_LANG_PROGRAM(
        [[
          #include <openssl/ssl.h>

          void foobar() {}
        ]],
        [[
          SSL_CTX *ctx;
        ]]
      ),
      [AC_MSG_RESULT([yes])],
      [AC_MSG_FAILURE([cannot link with OpenSSL library])]
    )

    dnl Set variables
    AC_SUBST([SSL_LIB], ["$OPENSSL_LIBS"])
    LDFLAGS="$saved_LDFLAGS"
    dnl CXXFLAGS="$saved_CXXFLAGS"

  ]
)

