SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/02_comm \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/07_TimeBase \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/11_Compression

CLEANFILES = *.C *.h

noinst_LTLIBRARIES = libTIDorbIdl_Compression.la 

idl_source = Compression.idl ZIOP.idl

idl_cpp_headers = \
        Compression.h \
        ZIOP.h

idl_cpp_sources = \
        Compression.C \
        ZIOP.C 

$(idl_cpp_sources): $(srcdir)/Compression.idl $(srcdir)/ZIOP.idl
	${IDLC} -CORBA_IDL -I./ -I$(srcdir)/../07_TimeBase \
	  -I$(srcdir)/../02_comm $(srcdir)/Compression.idl
	${IDLC} -CORBA_IDL -I./ -I$(srcdir)/../07_TimeBase \
	  -I$(srcdir)/../02_comm $(srcdir)/ZIOP.idl

libTIDorbIdl_Compression_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources) 

instdir=$(includedir)

inst_HEADERS = \
        $(idl_cpp_headers)

EXTRA_DIST = $(idl_source)
