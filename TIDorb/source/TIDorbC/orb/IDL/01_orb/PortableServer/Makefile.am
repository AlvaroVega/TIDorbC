SUBDIRS = Current POA POAManager .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
   IdUniquenessPolicyValue.h \
   POAManager.h \
   AdapterActivator_ext.h \
   ObjectId_ext.h \
   AdapterActivator.h \
   POAManager_ext.h \
   ForwardRequest.h \
   IdAssignmentPolicy_ext.h \
   IdAssignmentPolicyValue_ext.h \
   LifespanPolicy.h \
   Current.h \
   REQUEST_PROCESSING_POLICY_ID.h \
   ServantLocator.h \
   LifespanPolicy_ext.h \
   RequestProcessingPolicyValue.h \
   Current_ext.h \
   ThreadPolicy.h \
   POA.h \
   IdUniquenessPolicyValue_ext.h \
   ImplicitActivationPolicyValue_ext.h \
   ThreadPolicyValue_ext.h \
   RequestProcessingPolicy_ext.h \
   LIFESPAN_POLICY_ID.h \
   LifespanPolicyValue_ext.h \
   ServantActivator.h \
   LifespanPolicyValue.h \
   ID_ASSIGNMENT_POLICY_ID.h \
   IMPLICIT_ACTIVATION_POLICY_ID.h \
   ServantRetentionPolicyValue.h \
   ServantRetentionPolicyValue_ext.h \
   ServantManager_ext.h \
   RequestProcessingPolicyValue_ext.h \
   ThreadPolicy_ext.h \
   RequestProcessingPolicy.h \
   POAList_ext.h \
   ObjectId.h \
   ServantRetentionPolicy_ext.h \
   ImplicitActivationPolicy.h \
   ServantLocator_ext.h \
   ImplicitActivationPolicyValue.h \
   ServantManager.h \
   ImplicitActivationPolicy_ext.h \
   IdAssignmentPolicyValue.h \
   POAList.h \
   ServantRetentionPolicy.h \
   ServantActivator_ext.h \
   IdUniquenessPolicy_ext.h \
   IdUniquenessPolicy.h \
   THREAD_POLICY_ID.h \
   SERVANT_RETENTION_POLICY_ID.h \
   IdAssignmentPolicy.h \
   ID_UNIQUENESS_POLICY_ID.h \
   POA_ext.h \
   ForwardRequest_ext.h \
   ThreadPolicyValue.h

idl_cpp_extra_headers = \
   DynamicImplementation.h \
   ServantBase.h \
   Servant_var.h

idl_cpp_sources = \
   ThreadPolicyValue.C \
   IdUniquenessPolicyValue.C \
   POAManager.C \
   AdapterActivator.C \
   ForwardRequest.C \
   LifespanPolicy.C \
   Current.C \
   ServantLocator.C \
   RequestProcessingPolicyValue.C \
   ThreadPolicy.C \
   POA.C \
   ServantActivator.C \
   LifespanPolicyValue.C \
   ServantRetentionPolicyValue.C \
   RequestProcessingPolicy.C \
   ObjectId.C \
   ImplicitActivationPolicy.C \
   ImplicitActivationPolicyValue.C \
   ServantManager.C \
   IdAssignmentPolicyValue.C \
   POAList.C \
   ServantRetentionPolicy.C \
   IdUniquenessPolicy.C \
   IdAssignmentPolicy.C

instdir=$(includedir)/PortableServer

inst_HEADERS = \
   $(idl_cpp_headers) $(idl_cpp_extra_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_PortableServer.la

libTIDorbIdl_orb_PortableServer_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)

libTIDorbIdl_orb_PortableServer_la_LIBADD = \
   Current/libTIDorbIdl_orb_PortableServer_Current.la \
   POA/libTIDorbIdl_orb_PortableServer_POA.la \
   POAManager/libTIDorbIdl_orb_PortableServer_POAManager.la
