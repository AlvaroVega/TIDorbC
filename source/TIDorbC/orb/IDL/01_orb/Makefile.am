SUBDIRS = . CORBA PortableServer

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
	PortableServer_ext.h \
	PortableServer.h 
#	POA_CORBA.h

idl_cpp_sources = 

invalid_srcs = \
	$(top_srcdir)/src/idl/01_orb/CORBA/TypeCode.C \
	$(top_srcdir)/src/idl/01_orb/CORBA/ORB.C

IDLC_OPTIONS = -expanded -CORBA_IDL -no_skel
CORBAIDL=CORBA.idl
EXTRA=extra

if MINIMUN
IDLC_OPTIONS += -minimun -DMINIMUN
endif

$(idl_cpp_headers): $(srcdir)/$(CORBAIDL)
	if [ ! -f "CORBA.h" ]; then\
	  ${IDLC} $(IDLC_OPTIONS) -I./ $(srcdir)/$(CORBAIDL);\
		cp -R $(srcdir)/$(EXTRA)/*.h                $(builddir)/;\
		cp -R $(srcdir)/$(EXTRA)/CORBA/*.h          $(builddir)/CORBA/;\
		cp -R $(srcdir)/$(EXTRA)/CORBA/ORB/*.h      $(builddir)/CORBA/ORB/;\
		cp -R $(srcdir)/$(EXTRA)/PortableServer/*.h $(builddir)/PortableServer/;\
	fi;


nodist_inst_HEADERS = \
        $(idl_cpp_sources) \
	$(idl_source)

inst_HEADERS = \
	$(idl_cpp_headers)

instdir=$(includedir)

EXTRA_DIST = \
   $(idl_source) \
   $(EXTRA)/*.h \
   $(EXTRA)/CORBA/*.h \
   $(EXTRA)/CORBA/ORB/*.h \
   $(EXTRA)/PortableServer/*.h
