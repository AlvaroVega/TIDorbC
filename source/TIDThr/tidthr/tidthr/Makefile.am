
AM_CPPFLAGS = \
	-I${top_srcdir}/source/TIDThr/tidthr/include
#	-I$(top_srcdir) 
#	-I$(srcdir)/../include

lib_LTLIBRARIES = libtidthr.la

libtidthr_la_SOURCES = \
   Condition.C \
   Counter.C \
   ErrnoStr.C \
   Exception.C \
   Key.C \
   Monitor.C \
   Mutex.C \
   RecursiveMutex.C \
   RefCounter.C \
   Thread.C \
   ThreadGroup.C \
   ThreadGroupId.C \
   ThreadId.C

#libtidthr_la_LIBADD = @PTHREAD_LIB@
libtidthr_la_LIBADD = \
	$(PTHREAD_LIB)

libtidthr_la_LDFLAGS = \
	-avoid-version

