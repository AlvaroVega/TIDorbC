SUBDIRS = \
   01_orb \
   02_comm \
   03_sending_context \
   04_bidir_policy \
   05_cos_naming \
   06_dynamic_any \
   07_TimeBase \
   08_FT \
   09_PortableGroup \
   10_Messaging \
   11_Compression \
   12_Security \
   .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_srcdir)/source/TIDorbC/orb/include


invalid_srcs = \
   CORBA.gen/src/CORBA/TypeCode.C \
   CORBA.gen/src/CORBA/ORB.C

clean-local : #clean-recursive
	rm -rf $(idl_generateds) include

lib_LTLIBRARIES = libTIDorbIdl.la

libTIDorbIdl_la_SOURCES =

libTIDorbIdl_la_LIBADD = \
   $(top_builddir)/source/TIDorbC/orb/IDL/01_orb/CORBA/libTIDorbIdl_orb_CORBA.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/01_orb/PortableServer/libTIDorbIdl_orb_PortableServer.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/02_comm/libTIDorbIdl_comm.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/03_sending_context/libTIDorbIdl_sending_context.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/04_bidir_policy/libTIDorbIdl_bidir_policy.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/05_cos_naming/libTIDorbIdl_cos_naming.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/06_dynamic_any/libTIDorbIdl_dynamic_any.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/07_TimeBase/libTIDorbIdl_TimeBase.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/08_FT/libTIDorbIdl_FT.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/09_PortableGroup/libTIDorbIdl_PortableGroup.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/10_Messaging/libTIDorbIdl_Messaging.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/11_Compression/libTIDorbIdl_Compression.la \
   $(top_builddir)/source/TIDorbC/orb/IDL/12_Security/libTIDorbIdl_Security.la
#   $(top_builddir)/source/TIDorbC/orb/IDL/01_orb/POA_CORBA/libTIDorbIdl_orb_POA_CORBA.la

libTIDorbIdl_la_LDFLAGS = \
	-avoid-version


