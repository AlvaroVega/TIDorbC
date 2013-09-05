SUBDIRS = . 

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
	ObjectIdList_ext.h \
	ObjectIdList.h \
	InvalidName_ext.h \
	ObjectId.h \
	ObjectId_ext.h \
	InvalidName.h

idl_cpp_extra_headers = \
	RequestSeq.h

idl_cpp_sources = \
	ObjectIdList.C \
	ObjectId.C \
	InvalidName.C 

instdir=$(includedir)/CORBA/ORB

inst_HEADERS = \
	$(idl_cpp_headers) $(idl_cpp_extra_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_CORBA_ORB.la

libTIDorbIdl_orb_CORBA_ORB_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)
