SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/07_TimeBase

CLEANFILES = *.C *.h

noinst_LTLIBRARIES = libTIDorbIdl_TimeBase.la 

idl_source = TimeBase.idl

idl_cpp_headers = \
        TimeBase.h 

idl_cpp_sources = \
        TimeBase.C 

$(idl_cpp_sources): $(srcdir)/TimeBase.idl
	${IDLC} -CORBA_IDL -I./ $(srcdir)/TimeBase.idl

libTIDorbIdl_TimeBase_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources) 

instdir=$(includedir)

inst_HEADERS = \
        $(idl_cpp_headers)

EXTRA_DIST = $(idl_source)
