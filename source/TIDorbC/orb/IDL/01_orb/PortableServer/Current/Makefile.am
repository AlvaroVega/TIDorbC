SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb


CLEANFILES = *.C *.h

idl_cpp_headers = \
   NoContext_ext.h \
   NoContext.h

idl_cpp_sources = \
   NoContext.C

instdir=$(includedir)/PortableServer/Current

inst_HEADERS = \
   $(idl_cpp_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_PortableServer_Current.la

libTIDorbIdl_orb_PortableServer_Current_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)
