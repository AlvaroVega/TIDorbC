SUBDIRS = . 

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
	_service_detail_seq_ext.h \
	_service_detail_seq.h

idl_cpp_sources = \
	_service_detail_seq.C

instdir=$(includedir)/CORBA/ServiceDetail

inst_HEADERS = \
	$(idl_cpp_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_CORBA_ServiceDetail.la

libTIDorbIdl_orb_CORBA_ServiceDetail_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)
