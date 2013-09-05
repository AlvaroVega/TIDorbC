SUBDIRS = . 

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
	BadKind.h \
	Bounds.h \
	Bounds_ext.h \
	BadKind_ext.h

idl_cpp_sources = \
	BadKind.C \
	Bounds.C

instdir=$(includedir)/CORBA/TypeCode

inst_HEADERS = \
	$(idl_cpp_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_CORBA_TypeCode.la

libTIDorbIdl_orb_CORBA_TypeCode_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)
