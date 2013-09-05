SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/06_dynamic_any

CLEANFILES = *.C *.h

noinst_LTLIBRARIES = libTIDorbIdl_dynamic_any.la 

idl_source = DynamicAny.idl

idl_cpp_headers = \
        DynamicAny.h 

idl_cpp_sources = \
        DynamicAny.C

$(idl_cpp_sources): $(srcdir)/DynamicAny.idl
	${IDLC} -CORBA_IDL -I./ -I$(srcdir)/../07_TimeBase $(srcdir)/DynamicAny.idl

libTIDorbIdl_dynamic_any_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources) 

instdir=$(includedir)

inst_HEADERS = \
        $(idl_cpp_headers)

EXTRA_DIST = $(idl_source)
