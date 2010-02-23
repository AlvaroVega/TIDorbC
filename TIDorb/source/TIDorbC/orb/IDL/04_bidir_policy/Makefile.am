SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/04_bidir_policy

CLEANFILES = *.C *.h

noinst_LTLIBRARIES = libTIDorbIdl_bidir_policy.la

idl_source = BiDirPolicy.idl

idl_cpp_headers = \
   BiDirPolicy.h

idl_cpp_sources = \
   BiDirPolicy.C

$(idl_cpp_sources): $(srcdir)/BiDirPolicy.idl
	${IDLC} -CORBA_IDL -I./ $(srcdir)/BiDirPolicy.idl

libTIDorbIdl_bidir_policy_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)

instdir=$(includedir)

inst_HEADERS = \
   $(idl_cpp_headers)

EXTRA_DIST = $(idl_source)
