SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/05_cos_naming

CLEANFILES = *.C *.h

noinst_LTLIBRARIES = libTIDorbIdl_cos_naming.la

idl_source = CosNaming.idl

idl_cpp_headers = \
   CosNaming.h 
#   POA_CosNaming.h

idl_cpp_sources = \
   CosNaming.C 
#   POA_CosNaming.C

$(idl_cpp_sources): $(srcdir)/CosNaming.idl
	${IDLC} -CORBA_IDL -no_skel -I./ $(srcdir)/CosNaming.idl

libTIDorbIdl_cos_naming_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)

instdir=$(includedir)

inst_HEADERS = \
   $(idl_cpp_headers)

EXTRA_DIST = $(idl_source)
