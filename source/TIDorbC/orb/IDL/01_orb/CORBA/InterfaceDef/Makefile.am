SUBDIRS = . 

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
	FullInterfaceDescription_ext.h \
	FullInterfaceDescription.h

idl_cpp_sources = \
	FullInterfaceDescription.C 

instdir=$(includedir)/CORBA/InterfaceDef

inst_HEADERS = \
	$(idl_cpp_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_CORBA_InterfaceDef.la

libTIDorbIdl_orb_CORBA_InterfaceDef_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)
