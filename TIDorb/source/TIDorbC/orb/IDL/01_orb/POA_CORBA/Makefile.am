SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
   ModuleDef.h \
   ConstantDef.h \
   SequenceDef.h \
   InterfaceDef.h \
   AttributeDef.h \
   DomainManager.h \
   TypedefDef.h \
   NativeDef.h \
   ArrayDef.h \
   ValueDef.h \
   ConstructionPolicy.h \
   Repository.h \
   IDLType.h \
   FixedDef.h \
   Contained.h \
   StringDef.h \
   WstringDef.h \
   UnionDef.h \
   ExceptionDef.h \
   Current.h \
   ValueBoxDef.h \
   OperationDef.h \
   ValueMemberDef.h \
   Container.h \
   Policy.h \
   IRObject.h \
   AliasDef.h \
   StructDef.h \
   PrimitiveDef.h \
   EnumDef.h

idl_cpp_sources = \
   ModuleDef.C \
   ConstantDef.C \
   SequenceDef.C \
   InterfaceDef.C \
   AttributeDef.C \
   DomainManager.C \
   TypedefDef.C \
   NativeDef.C \
   ArrayDef.C \
   ValueDef.C \
   ConstructionPolicy.C \
   Repository.C \
   IDLType.C \
   FixedDef.C \
   Contained.C \
   StringDef.C \
   WstringDef.C \
   UnionDef.C \
   ExceptionDef.C \
   Current.C \
   ValueBoxDef.C \
   OperationDef.C \
   ValueMemberDef.C \
   Container.C \
   Policy.C \
   IRObject.C \
   AliasDef.C \
   StructDef.C \
   PrimitiveDef.C \
   EnumDef.C

instdir=$(includedir)/POA_CORBA

inst_HEADERS = \
   $(idl_cpp_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_POA_CORBA.la

libTIDorbIdl_orb_POA_CORBA_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)

