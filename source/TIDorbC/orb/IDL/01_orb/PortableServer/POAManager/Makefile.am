SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
   AdapterInactive.h \
   State_ext.h \
   State.h \
   AdapterInactive_ext.h

idl_cpp_sources = \
   AdapterInactive.C \
   State.C

instdir=$(includedir)/PortableServer/POAManager

inst_HEADERS = \
   $(idl_cpp_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_PortableServer_POAManager.la

libTIDorbIdl_orb_PortableServer_POAManager_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)
