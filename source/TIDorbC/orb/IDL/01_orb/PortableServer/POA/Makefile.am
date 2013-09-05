SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
   AdapterAlreadyExists.h \
   WrongPolicy.h \
   WrongAdapter_ext.h \
   AdapterAlreadyExists_ext.h \
   InvalidPolicy_ext.h \
   ServantAlreadyActive_ext.h \
   AdapterNonExistent.h \
   NoServant.h \
   ObjectAlreadyActive.h \
   ServantAlreadyActive.h \
   WrongPolicy_ext.h \
   ObjectAlreadyActive_ext.h \
   InvalidPolicy.h \
   AdapterNonExistent_ext.h \
   WrongAdapter.h \
   ObjectNotActive_ext.h \
   NoServant_ext.h \
   ObjectNotActive.h \
   ServantNotActive_ext.h \
   ServantNotActive.h

idl_cpp_sources = \
   AdapterAlreadyExists.C \
   WrongPolicy.C \
   AdapterNonExistent.C \
   NoServant.C \
   ObjectAlreadyActive.C \
   ServantAlreadyActive.C \
   InvalidPolicy.C \
   WrongAdapter.C \
   ObjectNotActive.C \
   ServantNotActive.C


instdir=$(includedir)/PortableServer/POA

inst_HEADERS = \
   $(idl_cpp_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_PortableServer_POA.la

libTIDorbIdl_orb_PortableServer_POA_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)
