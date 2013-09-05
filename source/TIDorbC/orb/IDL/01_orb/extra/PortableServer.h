//
// PortableServer.h (module)
//

#ifndef _PORTABLE_SERVER_H_
#define _PORTABLE_SERVER_H_

#include "CORBA.h"
#include "TIDorb/portable.h"


namespace PortableServer
{
	// Begin of forward declarations

	class ServantBase;
	typedef ServantBase* Servant;

	class DynamicImplementation;

	class ForwardRequest;
	typedef ForwardRequest* ForwardRequest_ptr;
	typedef ::TIDorb::templates::VariableSizeT_var<ForwardRequest> ForwardRequest_var;
	typedef ::TIDorb::templates::VariableSizeT_out<ForwardRequest> ForwardRequest_out;

	class ThreadPolicy;
	typedef ThreadPolicy* ThreadPolicy_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<ThreadPolicy> ThreadPolicy_var;
	typedef ::TIDorb::templates::InterfaceT_out<ThreadPolicy> ThreadPolicy_out;

	class LifespanPolicy;
	typedef LifespanPolicy* LifespanPolicy_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<LifespanPolicy> LifespanPolicy_var;
	typedef ::TIDorb::templates::InterfaceT_out<LifespanPolicy> LifespanPolicy_out;

	class IdUniquenessPolicy;
	typedef IdUniquenessPolicy* IdUniquenessPolicy_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<IdUniquenessPolicy> IdUniquenessPolicy_var;
	typedef ::TIDorb::templates::InterfaceT_out<IdUniquenessPolicy> IdUniquenessPolicy_out;

	class IdAssignmentPolicy;
	typedef IdAssignmentPolicy* IdAssignmentPolicy_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<IdAssignmentPolicy> IdAssignmentPolicy_var;
	typedef ::TIDorb::templates::InterfaceT_out<IdAssignmentPolicy> IdAssignmentPolicy_out;

	class ImplicitActivationPolicy;
	typedef ImplicitActivationPolicy* ImplicitActivationPolicy_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<ImplicitActivationPolicy> ImplicitActivationPolicy_var;
	typedef ::TIDorb::templates::InterfaceT_out<ImplicitActivationPolicy> ImplicitActivationPolicy_out;

	class ServantRetentionPolicy;
	typedef ServantRetentionPolicy* ServantRetentionPolicy_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<ServantRetentionPolicy> ServantRetentionPolicy_var;
	typedef ::TIDorb::templates::InterfaceT_out<ServantRetentionPolicy> ServantRetentionPolicy_out;

	class RequestProcessingPolicy;
	typedef RequestProcessingPolicy* RequestProcessingPolicy_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<RequestProcessingPolicy> RequestProcessingPolicy_var;
	typedef ::TIDorb::templates::InterfaceT_out<RequestProcessingPolicy> RequestProcessingPolicy_out;

	class POAManager;
	typedef POAManager* POAManager_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<POAManager> POAManager_var;
	typedef ::TIDorb::templates::InterfaceT_out<POAManager> POAManager_out;

	class AdapterActivator;
	typedef AdapterActivator* AdapterActivator_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<AdapterActivator> AdapterActivator_var;
	typedef ::TIDorb::templates::InterfaceT_out<AdapterActivator> AdapterActivator_out;

	class ServantManager;
	typedef ServantManager* ServantManager_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<ServantManager> ServantManager_var;
	typedef ::TIDorb::templates::InterfaceT_out<ServantManager> ServantManager_out;

	class ServantActivator;
	typedef ServantActivator* ServantActivator_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<ServantActivator> ServantActivator_var;
	typedef ::TIDorb::templates::InterfaceT_out<ServantActivator> ServantActivator_out;

	class ServantLocator;
	typedef ServantLocator* ServantLocator_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<ServantLocator> ServantLocator_var;
	typedef ::TIDorb::templates::InterfaceT_out<ServantLocator> ServantLocator_out;

	class POA;
	typedef POA* POA_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<POA> POA_var;
	typedef ::TIDorb::templates::InterfaceT_out<POA> POA_out;

	class Current;
	typedef Current* Current_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<Current> Current_var;
	typedef ::TIDorb::templates::InterfaceT_out<Current> Current_out;
}

// End of forward declarations



namespace PortableServer
{
	#include "PortableServer/POAList.h"
	#include "PortableServer/ObjectId.h"
	#include "PortableServer/ObjectId_ext.h"
	#include "PortableServer/ForwardRequest.h"
	#include "PortableServer/THREAD_POLICY_ID.h"
	#include "PortableServer/LIFESPAN_POLICY_ID.h"
	#include "PortableServer/ID_UNIQUENESS_POLICY_ID.h"
	#include "PortableServer/ID_ASSIGNMENT_POLICY_ID.h"
	#include "PortableServer/IMPLICIT_ACTIVATION_POLICY_ID.h"
	#include "PortableServer/SERVANT_RETENTION_POLICY_ID.h"
	#include "PortableServer/REQUEST_PROCESSING_POLICY_ID.h"
	#include "PortableServer/ThreadPolicyValue.h"
	#include "PortableServer/ThreadPolicy.h"
	#include "PortableServer/LifespanPolicyValue.h"
	#include "PortableServer/LifespanPolicy.h"
	#include "PortableServer/IdUniquenessPolicyValue.h"
	#include "PortableServer/IdUniquenessPolicy.h"
	#include "PortableServer/IdAssignmentPolicyValue.h"
	#include "PortableServer/IdAssignmentPolicy.h"
	#include "PortableServer/ImplicitActivationPolicyValue.h"
	#include "PortableServer/ImplicitActivationPolicy.h"
	#include "PortableServer/ServantRetentionPolicyValue.h"
	#include "PortableServer/ServantRetentionPolicy.h"
	#include "PortableServer/RequestProcessingPolicyValue.h"
	#include "PortableServer/RequestProcessingPolicy.h"
	#include "PortableServer/POAManager.h"
	#include "PortableServer/AdapterActivator.h"
	#include "PortableServer/ServantManager.h"
	#include "PortableServer/ServantActivator.h"
	#include "PortableServer/ServantLocator.h"
	#include "PortableServer/POA.h"
	#include "PortableServer/Current.h"
}


#include "TIDorb/portable/ServantDelegate.h"


namespace PortableServer
{
	#include "PortableServer/ServantBase.h"
	#include "PortableServer/Servant_var.h"
	#include "PortableServer/DynamicImplementation.h"

	typedef Servant_var<ServantBase> ServantBase_var;

}


#include "PortableServer_ext.h"

#endif // _PORTABLE_SERVER_H_
