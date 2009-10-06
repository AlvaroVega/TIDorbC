/////////////////////////////////////////////////////////////////////////
//
// File:        GOAImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/poa/POAImpl.h"
#include "TIDorb/core/goa/GOAImpl.h"




/**
* Constructor.
* @param orb The ORB.
* @param adapter_name The POA name.
* @param a_POAManager The POA manager which this POA belongs to.
* @param policies The policies to be used in the POA.
* @param parent The parent POA of this POA.
* @exception PortableServer::POA::InvalidPolicy
*            When policies are invalid.
*/
TIDorb::core::goa::GOAImpl::GOAImpl(TIDorb::core::TIDORB* the_orb,
                                    const char* adapter_name,
                                    PortableServer::POAManager_ptr a_POAManager,
                                    const CORBA::PolicyList& policies,
                                    PortableServer::POA_ptr parent)
  throw (PortableServer::POA::InvalidPolicy,CORBA::SystemException)
  : TIDorb::core::poa::POAImpl(the_orb, adapter_name, a_POAManager, policies, parent)
{
 if (orb->trace != NULL){
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::GOAImpl() ...");
  }
}




/* Destructor */
TIDorb::core::goa::GOAImpl::~GOAImpl()
{
  GroupReferencesTable::iterator i;
  GroupReferencesTable::iterator end = _group_references_table.end();
  for (i = _group_references_table.begin(); i != end; i++)
    CORBA::release(i->second);
}




/**
* Create a GOA as a children of this GOA.
* @param adapter_name The name of the new GOA.
* @param a_POAManager The POA manager which the new GOA belongs to.
* @param policies The policies to be used in the new GOA.
* @exception PortableServer::POA::AdapterAlreadyExists
*            When this GOA has alreay a children named adapter_name.
* @exception PortableServer::POA::InvalidPolicy
*            When policies are invalid.
*/
PortableServer::POA_ptr
TIDorb::core::goa::GOAImpl::create_POA(const char* adapter_name,
                                       PortableServer::POAManager_ptr a_POAManager,
                                       const ::CORBA::PolicyList& policies)
  throw (PortableServer::POA::AdapterAlreadyExists,
         PortableServer::POA::InvalidPolicy,
         CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);
  
  if (orb->trace != NULL){
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::create_POA ...");
  }
  if (_destroyed) {
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);
  }

  if (adapter_name == NULL) {
    throw CORBA::BAD_PARAM("Null reference", 0, CORBA::COMPLETED_NO);
  }

  if (find_children(adapter_name) != NULL) {
    throw PortableServer::POA::AdapterAlreadyExists();
  }

  TIDorb::core::poa::POAManagerImpl* POAmgr = NULL;
  if (a_POAManager == NULL) {
    POAmgr = new TIDorb::core::poa::POAManagerImpl(orb);
  } else {
    //POAmgr = dynamic_cast<TIDorb::core::poa::POAManagerImpl*>(a_POAManager);
    POAmgr = (TIDorb::core::poa::POAManagerImpl*)(a_POAManager);
  }

  TIDorb::core::goa::GOAImpl* newGOA =
    new GOAImpl(orb, adapter_name, (PortableServer::POAManager*)POAmgr, policies, this);

  if (_destroying) {
    newGOA->destroy(false, false);
  } else {
    _children.push_back(newGOA);
  }


  if(a_POAManager == NULL) { // recently created

    TIDorb::core::PolicyContext* context = NULL;
    context = newGOA->getPolicyContext();

    if (context != NULL){
      CORBA::Policy_ptr queuePolicy = NULL;
      queuePolicy = context->getPolicy(Messaging::QUEUE_ORDER_POLICY_TYPE);

      if(queuePolicy != NULL)
        POAmgr->setQueueOrderPolicy(queuePolicy);  
    }
    

  }

  return newGOA;
}




/**
* Creates the root POA as a Group Object Adapter.
* This method should only be called by TIDorb::core::TIDORB::resolve_initial_references()
* @param orb The ORB.
* @see TIDorb::core::TIDORB#resolve_initial_references
*/
TIDorb::core::poa::POAImpl* TIDorb::core::goa::GOAImpl::createRootPOA(TIDorb::core::TIDORB* orb)
{
  if (orb->trace != NULL){
      orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::createRootPOA ...");
  } 
  TIDorb::core::goa::GOAImpl* rootPOA = NULL;
  CORBA::PolicyList policies;
  policies.length(8);
  policies[0] = new TIDorb::core::poa::policies::IdAssignmentPolicyImpl(PortableServer::SYSTEM_ID);
  policies[1] = new TIDorb::core::poa::policies::IdUniquenessPolicyImpl(PortableServer::UNIQUE_ID);
  policies[2] = new TIDorb::core::poa::policies::ImplicitActivationPolicyImpl(PortableServer::IMPLICIT_ACTIVATION);
  policies[3] = new TIDorb::core::poa::policies::LifespanPolicyImpl(PortableServer::TRANSIENT);
  policies[4] = new TIDorb::core::poa::policies::RequestProcessingPolicyImpl(PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY);
  policies[5] = new TIDorb::core::poa::policies::ServantRetentionPolicyImpl(PortableServer::RETAIN);
  policies[6] = new TIDorb::core::poa::policies::ThreadPolicyImpl(PortableServer::ORB_CTRL_MODEL);
  policies[7] = new TIDorb::core::BidirectionalPolicyImpl(BiDirPolicy::NORMAL);
  try {
    TIDorb::core::poa::POAManagerImpl* mgr = new TIDorb::core::poa::POAManagerImpl(orb);
    rootPOA = new GOAImpl(orb, "rootPOA" , mgr, policies, NULL);

  } catch (const PortableServer::POA::InvalidPolicy& e) {
    // this can never happen
  }

  return rootPOA;
}




/**
* Get the object reference of a given servant.
* @param p_servant The servant.
* @exception PortableServer::POA::ServantNotActive
*            If p_servant is not active yet.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
CORBA::Object_ptr TIDorb::core::goa::GOAImpl::servant_to_reference(PortableServer::Servant p_servant)
  throw (PortableServer::POA::ServantNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (orb->trace != NULL){
      orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::servant_to_reference ...");
  } 

  // Try to get associated ObjectId (if possible, depends on POA policies)
  PortableServer::ObjectId_var oid = NULL;
  try {
    oid = servant_to_id(p_servant);
  } catch (...) {
  }
  
  // Servant may correspond to an ObjectId associated to an Group Object
  if (oid != NULL) {
    return id_to_reference(*oid);
  }

  // father->servant_to_reference()
  return TIDorb::core::poa::POAImpl::servant_to_reference(p_servant);
}




/**
* Get the object reference of a given object Id.
* @param oid The object Id.
* @exception PortableServer::POA::ObjectNotActive
*            If object is not active yet.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
CORBA::Object_ptr TIDorb::core::goa::GOAImpl::id_to_reference(const PortableServer::ObjectId& oid)
  throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  if (orb->trace != NULL){
      orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::id_to_reference ...");
  } 
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  // Look for ObjectId in table of Group Object references
  GroupReferencesTable::iterator pos;
  pos = _group_references_table.find(TIDorb::core::poa::OID(oid));
  if (pos != _group_references_table.end()) {
    //PRA
    return CORBA::Object::_duplicate(pos->second);
    //EPRA
  }
  return TIDorb::core::poa::POAImpl::id_to_reference(oid);
}



/**
* Deactivate an object in this POA.
* @param oid The object Id.
* @exception PortableServer::POA::ObjectNotActive
*            If id is not active in this POA.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
void TIDorb::core::goa::GOAImpl::deactivate_object(const PortableServer::ObjectId& oid)
  throw (PortableServer::POA::ObjectNotActive,
         PortableServer::POA::WrongPolicy,
         CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);
 
  if (orb->trace != NULL){
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::desactivate_object ...");
  } 
  TIDorb::core::poa::POAImpl::deactivate_object(oid);

  GroupReferencesTable::iterator pos;
  pos = _group_references_table.find(TIDorb::core::poa::OID(oid));
  if (pos != _group_references_table.end()) {
    CORBA::Object_ptr obj = pos->second;
    try {
      disassociate_reference_with_id(obj, oid);
    } catch (const PortableGroup::NotAGroupObject& ex) { /*unreachable*/
      if (orb->trace != NULL){
        orb->print_trace(TIDorb::util::TR_ERROR, 
                         "GOAImpl::desactivate_object catched a NotGroupObject exc");
      } 
    }
  }
}




/**
 * Returns UIPMC profile of the reference
 */
const TIDorb::core::comm::miop::ProfileMIOP*
TIDorb::core::goa::GOAImpl::get_group_object_profile(CORBA::Object_ptr the_ref)
{
  if (orb->trace != NULL){
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::get_group_object_profile ...");
  } 
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  if (the_ref == NULL)
    throw CORBA::BAD_PARAM("Null reference", 0, CORBA::COMPLETED_NO);

  try {
    TIDorb::portable::Stub* stub = dynamic_cast<TIDorb::portable::Stub*>(the_ref);

    if (!stub) {
      if (orb->trace != NULL){
        orb->print_trace(TIDorb::util::TR_ERROR, 
                         "GOAImpl::get_group_object_profile throws NotGroupObject");
      } 
      throw PortableGroup::NotAGroupObject();
    }

    //jagd
    //TIDorb::core::ObjectDelegateImpl* delegate =
    //  dynamic_cast<TIDorb::core::ObjectDelegateImpl*> (stub->_get_delegate());
    TIDorb::core::ObjectDelegateImpl* delegate =
      (TIDorb::core::ObjectDelegateImpl*) (stub->_get_delegate());

    return delegate->getReference()->profile_MIOP();

  } catch (const CORBA::Exception) {
    if (orb->trace != NULL){
      orb->print_trace(TIDorb::util::TR_ERROR, 
                       "GOAImpl::get_group_object_profile throws NotGroupObject after a CORBA::Exception");
    }
    throw PortableGroup::NotAGroupObject();
  }

  return NULL;
}




/**
 * Returns group object id from the UIPMC profile of the reference
 */
const char* TIDorb::core::goa::GOAImpl::get_group_object_id(CORBA::Object_ptr the_ref)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (orb->trace != NULL){
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::get_group_object_id ...");
  } 
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  if (the_ref == NULL)
    throw CORBA::BAD_PARAM("Null reference", 0, CORBA::COMPLETED_NO);

  const TIDorb::core::comm::miop::ProfileMIOP* profile =
    get_group_object_profile(the_ref);

  const char* group_object_id =
    TIDorb::core::comm::miop::MIOPCorbaloc::get_group_object_id(profile);

  if (group_object_id == NULL) {
    if (orb->trace != NULL){
      orb->print_trace(TIDorb::util::TR_ERROR, 
                       "GOAImpl::get_group_object_id throws NotGroupObject");
    } 
    throw PortableGroup::NotAGroupObject();
  }

  return group_object_id;
}




void TIDorb::core::goa::GOAImpl::group_object_POAKeys(const string& group_object_id, TIDorb::core::poa::POAKeyList& keys)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (orb->trace != NULL){
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::group_object_POAKeys ...");
  } 
  // Get ObjectIds associated to group object in this GOA
  PortableGroup::IDs_var ids = group_object_id_to_IDs(group_object_id);

  // Add POAKeys to list
  for (CORBA::ULong i = 0; i < ids->length(); i++) {
    TIDorb::core::poa::OID* oid = new TIDorb::core::poa::OID((*ids)[i]);
    keys.push_back(new TIDorb::core::poa::POAKey(this, the_id(), oid));
  }

  // Iterate through children
  //jagd 
  //vector<PortableServer::POA_var>::iterator it;
  vector<TIDorb::core::poa::POAImpl*>::iterator it;
  vector<TIDorb::core::poa::POAImpl*>::const_iterator end = _children.end();
  for (it = _children.begin(); it != end; it++) {
    TIDorb::core::goa::GOAImpl* goa =
      dynamic_cast<TIDorb::core::goa::GOAImpl*>((PortableServer::POA_ptr) *it);
    goa->group_object_POAKeys(group_object_id, keys);
  }
}




PortableGroup::IDs* TIDorb::core::goa::GOAImpl::group_object_id_to_IDs(const string& group_object_id)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (orb->trace != NULL){
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::group_object_id_to_IDs ...");
  } 
  GroupObjectIDsTable::iterator pos;
  pos = _group_object_ids_table.find(group_object_id);
  bool found = (pos != _group_object_ids_table.end());

  return (found) ? new PortableGroup::IDs(pos->second) : new PortableGroup::IDs(0);
}




// PortableGroup operations




PortableServer::ObjectId* TIDorb::core::goa::GOAImpl::create_id_for_reference(CORBA::Object_ptr the_ref)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  // Check for Group Object reference and get MIOP profile
  const TIDorb::core::comm::miop::ProfileMIOP* profile =
    get_group_object_profile(the_ref);

  // Generate unique ObjectId
  TIDorb::core::poa::OID* next_poa_oid = nextOID();
  PortableServer::ObjectId* oid = next_poa_oid->toByteArray();

  // Register Group Object reference for further id_to_reference() operations
  _group_references_table[*next_poa_oid] = CORBA::Object::_duplicate(the_ref);
  if (orb->trace != NULL){
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::create_id_for_reference ...");
  }
  // Create a MIOP connection listening to reference's multicast group,
  // or bound to unicast address (NON-STANDARD FEATURE!)
  try {
    orb->getCommunicationManager()->getMulticastLayer()->join_group(profile->getListenPoint());
  } catch (CORBA::SystemException& e) {
    delete next_poa_oid;
    delete oid;
    if (orb->trace != NULL){
      orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "create_id_for_reference exception");
    }
    throw CORBA::INITIALIZE(e.what());  	
  }

  // Associate ObjectId to Group Object reference
  associate_reference_with_id(the_ref, *oid);

  delete next_poa_oid;
  return oid;
}




PortableGroup::IDs* TIDorb::core::goa::GOAImpl::reference_to_ids(CORBA::Object_ptr the_ref)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (orb->trace != NULL){
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::reference_to_ids ...");
  } 
  string group_object_id = get_group_object_id(the_ref);
  return group_object_id_to_IDs(group_object_id);
}




void TIDorb::core::goa::GOAImpl::associate_reference_with_id(CORBA::Object_ptr the_ref, const PortableServer::ObjectId& oid)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (orb->trace != NULL){
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::associate_reference_with_id ...");
  } 
  string group_object_id = get_group_object_id(the_ref);

  PortableGroup::IDs& ids = _group_object_ids_table[group_object_id];

  CORBA::ULong len = ids.length();
  for (CORBA::ULong i = 0; i < len; i++) {
    if (TIDorb::core::poa::OID(ids[i]) == TIDorb::core::poa::OID(oid)) return;
  }

  ids.length(len + 1);
  ids[len] = oid;
}




void TIDorb::core::goa::GOAImpl::disassociate_reference_with_id(CORBA::Object_ptr the_ref, const PortableServer::ObjectId& oid)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (orb->trace != NULL){
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "GOAImpl::disassociate_reference_with_id ...");
  } 
  string group_object_id = get_group_object_id(the_ref);

  GroupObjectIDsTable::iterator pos;
  pos = _group_object_ids_table.find(group_object_id);
  bool found = (pos != _group_object_ids_table.end());

  if (found) {
    PortableGroup::IDs& ids = pos->second;
    CORBA::ULong len = ids.length();
    for (CORBA::ULong i = 0; i < len; i++) {
      if (TIDorb::core::poa::OID(ids[i]) == TIDorb::core::poa::OID(oid)) {
        ids[i] = ids[len - 1];
        ids.length(len - 1);
        return;
      }
    }
  }
}
