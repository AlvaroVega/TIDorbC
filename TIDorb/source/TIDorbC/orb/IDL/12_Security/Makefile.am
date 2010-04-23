SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/02_comm \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/07_TimeBase \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/12_Security

CLEANFILES = *.C *.h

noinst_LTLIBRARIES = libTIDorbIdl_Security.la 

idl_source = \
	CSI.idl \
	CSIIOP.idl \
	GSSUP.idl \
	Security.idl \
	SecurityLevel1.idl \
	SecurityLevel2.idl \
	SecurityLevel3.idl \
	SSLIOP.idl

idl_cpp_headers = \
	CSI.h \
	CSIIOP.h \
	GSSUP.h \
	Security.h \
	SecurityLevel1.h \
	SecurityLevel2.h \
	POA_SecurityLevel2.h \
	SecurityLevel3.h \
	OBV_SecurityLevel3.h \
	SSLIOP.h

idl_cpp_sources = \
	CSI.C \
	CSIIOP.C \
	GSSUP.C \
	Security.C \
	SecurityLevel1.C \
	SecurityLevel2.C \
	POA_SecurityLevel2.C \
	SecurityLevel3.C \
	OBV_SecurityLevel3.C \
	SSLIOP.C

$(idl_cpp_sources): $(srcdir)/CSI.idl $(srcdir)/CSIIOP.idl $(srcdir)/GSSUP.idl $(srcdir)/Security.idl $(srcdir)/SecurityLevel1.idl $(srcdir)/SecurityLevel2.idl $(srcdir)/SecurityLevel3.idl $(srcdir)/SSLIOP.idl
	${IDLC} -CORBA_IDL -I./ -I$(srcdir)/../07_TimeBase \
	  -I$(srcdir)/../02_comm $(srcdir)/CSI.idl
	${IDLC} -CORBA_IDL -I./ -I$(srcdir)/../07_TimeBase \
	  -I$(srcdir)/../02_comm $(srcdir)/CSIIOP.idl
	${IDLC} -CORBA_IDL -I./ -I$(srcdir)/../07_TimeBase \
	  -I$(srcdir)/../02_comm $(srcdir)/GSSUP.idl
	${IDLC} -CORBA_IDL -I./ -I$(srcdir)/../07_TimeBase \
	  -I$(srcdir)/../02_comm $(srcdir)/Security.idl
	${IDLC} -CORBA_IDL -I./ -I$(srcdir)/../07_TimeBase \
	  -I$(srcdir)/../02_comm $(srcdir)/SecurityLevel1.idl
	${IDLC} -CORBA_IDL -I./ -I$(srcdir)/../07_TimeBase \
	  -I$(srcdir)/../02_comm $(srcdir)/SecurityLevel2.idl
	${IDLC} -CORBA_IDL -I./ -I$(srcdir)/../07_TimeBase \
	  -I$(srcdir)/../02_comm $(srcdir)/SecurityLevel3.idl
	${IDLC} -CORBA_IDL -I./ -I$(srcdir)/../07_TimeBase \
	  -I$(srcdir)/../02_comm $(srcdir)/SSLIOP.idl


libTIDorbIdl_Security_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources) 

instdir=$(includedir)

inst_HEADERS = \
        $(idl_cpp_headers)

EXTRA_DIST = $(idl_source)
