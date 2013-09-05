SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/09_PortableGroup

CLEANFILES = *.C *.h

noinst_LTLIBRARIES = libTIDorbIdl_PortableGroup.la 

idl_source = PortableGroup.idl

idl_cpp_headers = \
        PortableGroup.h 

idl_cpp_sources = \
        PortableGroup.C 

$(idl_cpp_sources): $(srcdir)/PortableGroup.idl
	${IDLC} -CORBA_IDL -I./ $(srcdir)/PortableGroup.idl

libTIDorbIdl_PortableGroup_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources) 

instdir=$(includedir)

inst_HEADERS = \
        $(idl_cpp_headers)

EXTRA_DIST = $(idl_source)
