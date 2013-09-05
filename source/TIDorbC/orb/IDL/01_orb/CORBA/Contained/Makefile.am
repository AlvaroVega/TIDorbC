SUBDIRS = . 

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
	Description_ext.h \
	Description.h

idl_cpp_sources = \
	Description.C 

instdir=$(includedir)/CORBA/Contained

inst_HEADERS = \
	$(idl_cpp_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_CORBA_Contained.la

libTIDorbIdl_orb_CORBA_Contained_la_SOURCES = \
	$(idl_cpp_headers) \
	$(idl_cpp_sources)

