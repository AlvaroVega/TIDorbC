SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/03_sending_context

CLEANFILES = *.C *.h

noinst_LTLIBRARIES = libTIDorbIdl_sending_context.la

idl_source = SendingContext.idl

idl_cpp_headers = \
   SendingContext.h
#   POA_SendingContext.h

idl_cpp_sources = \
   SendingContext.C
#   POA_SendingContext.C

$(idl_cpp_sources): $(srcdir)/SendingContext.idl
	${IDLC} -CORBA_IDL -no_skel -I./ $(srcdir)/SendingContext.idl

libTIDorbIdl_sending_context_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)

instdir=$(includedir)

inst_HEADERS = \
   $(idl_cpp_headers)

EXTRA_DIST = $(idl_source)
