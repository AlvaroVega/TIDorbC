SUBDIRS = . 

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
	_service_details_seq_ext.h \
	_service_options_seq_ext.h \
	_service_details_seq.h \
	_service_options_seq.h

idl_cpp_sources = \
	_service_options_seq.C \
	_service_details_seq.C

instdir=$(includedir)/CORBA/ServiceInformation

inst_HEADERS = \
	$(idl_cpp_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_CORBA_ServiceInformation.la

libTIDorbIdl_orb_CORBA_ServiceInformation_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)
