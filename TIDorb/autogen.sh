#!/bin/sh

# Simple script to generate autotool files

print_exec_result() {
  if [ "$?" -eq "0" ]; then
    echo "OK"
  else
    echo "failed, see autogen.log for more details"
    exit -1
  fi
}

autoreconf --install 2>&1 >> autogen.log

printf "Expanding M4 files... "
aclocal-1.11 -I m4 2>&1 > autogen.log
print_exec_result

printf "Executing autoconf... "
autoconf 2>&1 >> autogen.log
print_exec_result

printf "Executing automake... "
automake-1.11 --add-missing 2>&1 >> autogen.log
print_exec_result

