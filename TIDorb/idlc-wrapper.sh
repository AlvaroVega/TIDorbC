#!/bin/sh
#
# TID IDL compiler wrapper for TIDorbC
# Copyright (c) 2008 Telefonica Investigacion y Desarrollo
# All rights reserved
#
# Written by Alvaro Polo <apv@tid.es>
#
# This script is used as a wrapper for TIDIdlc Java jar to avoid
# the constant use of environment variables JAVA_HOME and
# TIDIDLC_HOME used in idl2cpp script distributed along TIDIdlc.
#


usage() {
   echo "TID IDL compiler wrapper for TIDorbC" >&2
   echo "Copyright (c) 2008 Telefonica I+D" >&2
   echo "All rights reserved" >&2
   echo "" >&2
   
   [ "$1" ] && echo "Error: $1"
   
   echo "Usage: $0 [<options>] <compiler arguments>"
   echo ""
   echo "Options: --java-bin=<path>     Java VM binary location"
   echo "         --tididlc-home=<dir>  TIDIdlc home directory"
   echo ""
   exit -1
}

# Script entry point.

java_bin=""
tididlc_home=""
for arg in $*; do
   key=`echo "$arg" | cut -d= -f1`
   value=`echo "$arg" | cut -d= -f2`
   
   case "$key" in
      --java-bin)     java_bin=$value;;
      --tididlc-home) tididlc_home=$value;;
      *)              args="$args $value";;
   esac
done

[ "$java_bin" ] && java="$java_bin"
[ -x "$java" ] || java="$HOME/bin/java"
[ -x "$java" ] || java="/usr/local/bin/java"
[ -x "$java" ] || java="/usr/bin/java"

eval "$java -version 2>/dev/null >/dev/null" || usage "Java VM cannot be executed"

[ "$tididlc_home" ] && tididlc_jar="$tididlc_home/lib/idl2cpp.jar"
[ -f "$tididlc_jar" ] || tididlc_jar="$HOME/lib/idl2cpp.jar"
[ -f "$tididlc_jar" ] || tididlc_jar="/usr/local/lib/idl2cpp.jar"
[ -f "$tididlc_jar" ] || tididlc_jar="/usr/lib/idl2cpp.jar"
[ -f "$tididlc_jar" ] || usage "cannot find idl2cpp.jar"

[ "$args" ] && $java -jar $tididlc_jar $args
