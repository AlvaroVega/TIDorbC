SUBDIRS = . 

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
	Description_ext.h \
	Description.h \
	DescriptionSeq_ext.h \
	DescriptionSeq.h

idl_cpp_sources = \
	Description.C \
	DescriptionSeq.C

instdir=$(includedir)/CORBA/Container

inst_HEADERS = \
	$(idl_cpp_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_CORBA_Container.la

libTIDorbIdl_orb_CORBA_Container_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)
