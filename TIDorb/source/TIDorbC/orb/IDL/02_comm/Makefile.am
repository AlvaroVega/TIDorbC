SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/02_comm 

CLEANFILES = *.C *.h

noinst_LTLIBRARIES = libTIDorbIdl_comm.la

idl_source = IOP.idl

idl_cpp_headers = \
   IOP.h

idl_cpp_sources = \
   IOP.C

$(idl_cpp_sources): $(srcdir)/IOP.idl
	${IDLC} -CORBA_IDL -I./ $(srcdir)/IOP.idl

libTIDorbIdl_comm_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)

instdir=$(includedir)

inst_HEADERS = \
   $(idl_cpp_headers)

EXTRA_DIST = $(idl_source)
