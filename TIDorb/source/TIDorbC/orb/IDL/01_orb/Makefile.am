SUBDIRS = . CORBA POA_CORBA PortableServer

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(builddir)

CLEANFILES = *.C *.h

idl_source = $(srcdir)/CORBA.idl

idl_cpp_headers = \
	CORBA_ext.h \
	CORBA.h \
	_global_includes_for_CORBA_idl.h \
	POA_CORBA.h \
	PortableServer_ext.h \
	PortableServer.h 

idl_cpp_sources = 

invalid_srcs = \
	$(top_srcdir)/src/idl/01_orb/CORBA/TypeCode.C \
	$(top_srcdir)/src/idl/01_orb/CORBA/ORB.C

$(idl_cpp_headers): $(srcdir)/CORBA.idl
	if [ ! -f "CORBA.h" ]; then\
	  ${IDLC} -expanded -CORBA_IDL -I./ $(srcdir)/CORBA.idl;\
		cp -R $(srcdir)/extra/*.h                $(builddir)/;\
		cp -R $(srcdir)/extra/CORBA/*.h          $(builddir)/CORBA/;\
		cp -R $(srcdir)/extra/CORBA/ORB/*.h      $(builddir)/CORBA/ORB/;\
		cp -R $(srcdir)/extra/PortableServer/*.h $(builddir)/PortableServer/;\
	fi;


nodist_inst_HEADERS = \
        $(idl_cpp_sources) \
	$(idl_source)

inst_HEADERS = \
	$(idl_cpp_headers)

instdir=$(includedir)

EXTRA_DIST = \
   $(idl_source) \
   extra/*.h \
   extra/CORBA/*.h \
   extra/CORBA/ORB/*.h \
   extra/PortableServer/*.h
