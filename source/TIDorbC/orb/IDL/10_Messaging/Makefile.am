SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/02_comm \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/07_TimeBase \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/10_Messaging

CLEANFILES = *.C *.h

noinst_LTLIBRARIES = libTIDorbIdl_Messaging.la 

idl_source = Messaging.idl

idl_cpp_headers = \
        Messaging.h
#        POA_Messaging.h

idl_cpp_sources = \
        Messaging.C
#        POA_Messaging.C 

$(idl_cpp_sources): $(srcdir)/Messaging.idl
	${IDLC} -CORBA_IDL -no_skel -I./ -I$(srcdir)/../07_TimeBase \
	  -I$(srcdir)/../02_comm $(srcdir)/Messaging.idl

libTIDorbIdl_Messaging_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources) 

instdir=$(includedir)

inst_HEADERS = \
        $(idl_cpp_headers)

EXTRA_DIST = $(idl_source)
