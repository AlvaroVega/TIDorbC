/////////////////////////////////////////////////////////////////////////
//
// File:        POAImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2005
//
// Revised:
//
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/poa/POAImpl.h"


#ifdef TIDORB_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif

#include <sys/time.h>

TIDorb::core::poa::POAImpl::POAImpl(TIDorb::core::TIDORB* the_orb, const char* adapter_name,
                                    PortableServer::POAManager_ptr a_POAManager,
                                    const CORBA::PolicyList& policies, 
                                    PortableServer::POA_ptr parent)
throw (PortableServer::POA::InvalidPolicy,CORBA::SystemException)
: _destroyed(false), 
  _name (CORBA::string_dup(adapter_name)),
  //jagd
  //_parent (PortableServer::POA::_duplicate(parent)),
  _parent (parent),
  _POAManager (NULL),
  _completion (the_orb), 
  _destroying (false),
  _etherealize(false),
  _activator (NULL), _servant_manager (NULL),
  _servant (NULL), _activeObjectMap (NULL),
  _currentOID(NULL)
{
  orb = the_orb;
  the_orb->_add_ref();

  _IdAssignmentPolicy = NULL;
  _IdUniquenessPolicy = NULL;
  _ImplicitActivationPolicy = NULL;
  _LifespanPolicy = NULL;
  _RequestProcessingPolicy = NULL;
  _ServantRetentionPolicy = NULL;
  _ThreadPolicy = NULL;
  _BidirectionalPolicy = NULL;
  _poa_string_name = NULL;

  _poa_id = TIDorb::core::util::Time::currentTimeMillis();

  //jagd
  //_POAManager = dynamic_cast<TIDorb::core::poa::POAManagerImpl*>
  //               (PortableServer::POAManager::_duplicate(a_POAManager));
  //
  _POAManager = (TIDorb::core::poa::POAManagerImpl*)a_POAManager;
   //              (PortableServer::POAManager::_duplicate(a_POAManager));
  _POAManager->_add_ref();


  _POAManager->addPOA(this);

  _parent = parent;

  if (parent == NULL) {
    _path.length(0);
  } else {
    TIDorb::core::poa::POAImpl* parent_aux =
      dynamic_cast<TIDorb::core::poa::POAImpl*>(parent);
  
    //jagd
    parent_aux->_add_ref();

    CORBA::ULong length;
    length = parent_aux->the_path().length();
    _path.length(length+1);
    for (CORBA::ULong i=0; i < parent_aux->the_path().length(); i++)
      _path[i] = CORBA::string_dup(parent_aux->the_path()[i]);

    _path[length] = CORBA::string_dup(adapter_name);
  }
  TIDorb::core::PolicyContext* policy_context = new TIDorb::core::PolicyContext();
  _referencePolicies = new TIDorb::core::messaging::PoliciesComponent(policy_context); 

  // Set policies
  for (CORBA::ULong i=0; i<policies.length(); i++) {

    if(!(policies[i]))
      throw CORBA::BAD_PARAM("Null policy reference");

    switch (policies[i]->policy_type()) {
      case PortableServer::ID_ASSIGNMENT_POLICY_ID:
        if (_IdAssignmentPolicy != NULL)
          throw PortableServer::POA::InvalidPolicy();
        _IdAssignmentPolicy = 
          dynamic_cast<TIDorb::core::poa::policies::IdAssignmentPolicyImpl*>(policies[i]);
        break;

      case PortableServer::ID_UNIQUENESS_POLICY_ID:
        if (_IdUniquenessPolicy != NULL)
          throw PortableServer::POA::InvalidPolicy();
        _IdUniquenessPolicy = 
          dynamic_cast<TIDorb::core::poa::policies::IdUniquenessPolicyImpl*>(policies[i]);
        break;

      case PortableServer::IMPLICIT_ACTIVATION_POLICY_ID:
        if (_ImplicitActivationPolicy != NULL)
          throw PortableServer::POA::InvalidPolicy();
        _ImplicitActivationPolicy = 
          dynamic_cast<TIDorb::core::poa::policies::ImplicitActivationPolicyImpl*>(policies[i]);
        break;

      case PortableServer::LIFESPAN_POLICY_ID:
        if (_LifespanPolicy != NULL)
          throw PortableServer::POA::InvalidPolicy();
        _LifespanPolicy = 
          dynamic_cast<TIDorb::core::poa::policies::LifespanPolicyImpl*>(policies[i]);
        break;

      case PortableServer::REQUEST_PROCESSING_POLICY_ID:
        if (_RequestProcessingPolicy != NULL)
          throw PortableServer::POA::InvalidPolicy();
        _RequestProcessingPolicy = 
          dynamic_cast<TIDorb::core::poa::policies::RequestProcessingPolicyImpl*>(policies[i]);
        break;

      case PortableServer::SERVANT_RETENTION_POLICY_ID:
        if (_ServantRetentionPolicy != NULL)
          throw PortableServer::POA::InvalidPolicy();
        _ServantRetentionPolicy = 
          dynamic_cast<TIDorb::core::poa::policies::ServantRetentionPolicyImpl*>(policies[i]);
        break;

      case PortableServer::THREAD_POLICY_ID:
        if (_ThreadPolicy != NULL)
          throw PortableServer::POA::InvalidPolicy();
        _ThreadPolicy = 
          dynamic_cast<TIDorb::core::poa::policies::ThreadPolicyImpl*>(policies[i]);
        break;

      case BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE:
        if (_BidirectionalPolicy != NULL)
          throw PortableServer::POA::InvalidPolicy();
        _BidirectionalPolicy = 
          dynamic_cast<TIDorb::core::BidirectionalPolicyImpl*>(policies[i]);
        break;

    case ZIOP::COMPRESSION_ENABLING_POLICY_ID: 
    case ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID: 
    case ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID:
    case ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID:
      try {
        _referencePolicies->getPolicies()->setPolicy(policies[i]);
      }
      catch (const CORBA::PolicyError& e) {
        throw PortableServer::POA::InvalidPolicy();
      }       
      break;

    default:                        
      try {
        _referencePolicies->getPolicies()->setPolicy(policies[i]);
      }
      catch (const CORBA::PolicyError& e) {
        throw PortableServer::POA::InvalidPolicy();
      }       

    }
  }

  // Set default policies
  if (_IdAssignmentPolicy == NULL)
    _IdAssignmentPolicy = (TIDorb::core::poa::policies::IdAssignmentPolicyImpl*)
      create_id_assignment_policy(PortableServer::SYSTEM_ID);
  if (_IdUniquenessPolicy == NULL)
    _IdUniquenessPolicy = (TIDorb::core::poa::policies::IdUniquenessPolicyImpl*)
      create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
  if (_ImplicitActivationPolicy == NULL)
    _ImplicitActivationPolicy = (TIDorb::core::poa::policies::ImplicitActivationPolicyImpl*)
      create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION);
  if (_LifespanPolicy == NULL)
    _LifespanPolicy = (TIDorb::core::poa::policies::LifespanPolicyImpl*) 
      create_lifespan_policy(PortableServer::TRANSIENT);
  if (_RequestProcessingPolicy == NULL)
    _RequestProcessingPolicy = (TIDorb::core::poa::policies::RequestProcessingPolicyImpl*)
      create_request_processing_policy(PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY);
  if (_ServantRetentionPolicy == NULL)
    _ServantRetentionPolicy = (TIDorb::core::poa::policies::ServantRetentionPolicyImpl*)
      create_servant_retention_policy(PortableServer::RETAIN);
  if (_ThreadPolicy == NULL)
    _ThreadPolicy = (TIDorb::core::poa::policies::ThreadPolicyImpl*)
      create_thread_policy(PortableServer::ORB_CTRL_MODEL);
  if (_BidirectionalPolicy == NULL)
    _BidirectionalPolicy = new TIDorb::core::BidirectionalPolicyImpl(BiDirPolicy::NORMAL);
  if(_ServantRetentionPolicy->value() == PortableServer::RETAIN)
    _activeObjectMap = new ActiveObjectMap();
  if(_IdAssignmentPolicy->value() == PortableServer::SYSTEM_ID) {
    _currentOID = new SystemOID(_poa_id, 0);
  }
  if(_ThreadPolicy->value() == PortableServer::SINGLE_THREAD_MODEL)
    single_thread_recursive_mutex = new TIDThr::RecursiveMutex();

// Wrong Policies
// TODO: delete objects before throwing exception
  if((_ImplicitActivationPolicy->value() == PortableServer::IMPLICIT_ACTIVATION)
      && ((_IdAssignmentPolicy->value() == PortableServer::USER_ID)
      || (_ServantRetentionPolicy->value() == PortableServer::NON_RETAIN)))
    throw PortableServer::POA::InvalidPolicy();

  if ((_ServantRetentionPolicy->value() == PortableServer::NON_RETAIN)
    && (_RequestProcessingPolicy->value() == PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY))
    throw PortableServer::POA::InvalidPolicy();
};


TIDorb::core::poa::POAImpl::~POAImpl()
{
  //jagd destruimos el array
   for (size_t i = 0; i < _children.size(); i++) {
     _children[i]->_remove_ref();
   }
   
  CORBA::release(_POAManager);
  //delete _activeObjectMap; -> Delete move to  trueDestroy()
  delete _IdAssignmentPolicy;
  delete _IdUniquenessPolicy;
  delete _ImplicitActivationPolicy;
  delete _LifespanPolicy;
  delete _RequestProcessingPolicy;
  delete _ServantRetentionPolicy;
  delete _ThreadPolicy;
  delete _BidirectionalPolicy;
  delete _referencePolicies;
  //PRA
  delete _currentOID;
  if (_servant) {
    _servant->_remove_ref();
  }
  //EPRA
  CORBA::string_free(_poa_string_name);
  CORBA::release(orb);
};


/**
* Removes an object if there are no executing request using it.
* @param oid The object Id.
*/
void TIDorb::core::poa::POAImpl::tryToRemoveObject(const TIDorb::core::poa::OID& oid)
throw (PortableServer::POA::ObjectNotActive)
{
  if (_ServantRetentionPolicy->value() == PortableServer::RETAIN) {
//     if (_activeObjectMap->isActive(oid)) {
//       bool OidNotInUse = _activeObjectMap->removeUser(oid);
//       if (OidNotInUse) {
//         trueRemoveObject(oid);
//       }
//     }
    bool OidNotInUse = _activeObjectMap->removeUser_if_Active(oid);
    if (OidNotInUse) {
      trueRemoveObject(oid);
    }
  }
};

/**
* Removes an object.
* @param oid The object Id.
*/
void TIDorb::core::poa::POAImpl::trueRemoveObject(const TIDorb::core::poa::OID& oid)
throw (PortableServer::POA::ObjectNotActive )
{
  // Remove OID from ActiveObjectMap 

  
  // PortableServer::Servant servant = _activeObjectMap->get(oid);
  PortableServer::Servant servant = _activeObjectMap->get_and_remove(oid);
  
  // add reference counting to the servant before erase from the AOM
  //servant->_add_ref();
      
  if (orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << toString() << ": Removing object [" << oid.toString() << "]";
    orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }  
  // remove makes servant->_remove_ref();
  //_activeObjectMap->remove(oid);


  // Try to etherealize, if necessary
  if ((_RequestProcessingPolicy->value() == PortableServer::USE_SERVANT_MANAGER) &&
  (_servant_manager != NULL)) {
    PortableServer::ServantActivator_ptr activator =
      PortableServer::ServantActivator::_narrow(_servant_manager);
    if (isSingleThread()) {
      TIDThr::Synchronized synchro(*single_thread_recursive_mutex);

      activator->etherealize(oid.get_object_id(), this, servant, true, _activeObjectMap->contains(servant));
    } else {
      activator->etherealize(oid.get_object_id(), this, servant, true, _activeObjectMap->contains(servant));
    }
  }
  // Continue with waiting activations of oid (if any)
  {
    TIDThr::Synchronized synchro(recursive_mutex);
    TIDorb::portable::ServantDelegate* aux_delegate=servant->get_delegate();
    //jagd 
    //TIDorb::core::poa::ServantDelegate* delegate=dynamic_cast<TIDorb::core::poa::ServantDelegate*>(aux_delegate);
    TIDorb::core::poa::ServantDelegate* delegate=(TIDorb::core::poa::ServantDelegate*)(aux_delegate);
    delegate->notifyAll();
    servant->_remove_ref();
  }
};

/**
* Find a POA among the POA's children.
* @param poa_name The POA name.
*/
//jagd
//PortableServer::POA_ptr TIDorb::core::poa::POAImpl::find_children(const char* poa_name) {
TIDorb::core::poa::POAImpl * TIDorb::core::poa::POAImpl::find_children(const char* poa_name) {
  //jagd 
  //PortableServer::POA_ptr poa = NULL;
  TIDorb::core::poa::POAImpl * poa = NULL;
  for (size_t i=0; i<_children.size(); i++) {
    //jagd 
    //poa = (PortableServer::POA*)_children[i];
    poa = _children[i];
    if (strcmp(poa_name,poa->the_name())==0) {
      return poa;
    }
  }
  return NULL;
};

/**
* Returns the repository Id of a servant.
* @param servant The servant.
* @return The repository Id.
*/
const char* TIDorb::core::poa::POAImpl::getRepositoryId(PortableServer::Servant servant) {
  TIDorb::portable::ServantDelegate* aux_delegate=servant->get_delegate();
  //jagd
  //TIDorb::core::poa::ServantDelegate* delegate=dynamic_cast<TIDorb::core::poa::ServantDelegate*>(aux_delegate);
  TIDorb::core::poa::ServantDelegate* delegate=(TIDorb::core::poa::ServantDelegate*)(aux_delegate);

  const PortableServer::ObjectId& __oid = delegate->object_id(servant);
  const CORBA::RepositoryIdSeq_ptr __seq = servant->_all_interfaces(this, __oid);
  CORBA::RepositoryId __id = (*__seq)[0];
  return __id;
  //  return (*(servant->_all_interfaces(this, delegate->object_id(servant))))[0];
};

/**
 * Returns the policy context
 */
TIDorb::core::PolicyContext* TIDorb::core::poa::POAImpl::getPolicyContext()
{        
  return _referencePolicies->getPolicies();
}


/**
* Activate an object in this POA.
* @param id User created object Id.
* @param p_servant Servant.
* @exception PortableServer::POA::ObjectAlreadyActive
*            If id is already active in this POA.
* @exception PortableServer::POA::ServantAlreadyActive
*            If p_servant is already active in this POA.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
void TIDorb::core::poa::POAImpl::activate_object_with_id(const TIDorb::core::poa::OID& oid,PortableServer::Servant p_servant)
throw (PortableServer::POA::ServantAlreadyActive,PortableServer::POA::ObjectAlreadyActive,
       PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if (_ServantRetentionPolicy->value() != PortableServer::RETAIN) {
    throw PortableServer::POA::WrongPolicy();
  }
  bool uniqueId = _IdUniquenessPolicy->value() == PortableServer::UNIQUE_ID;
  // See if there a destruction process running

  {
    TIDThr::Synchronized synchro(recursive_mutex);
    //java.lang.Object semaphore = _activeObjectMap.isDestroying(oid);
    TIDorb::core::poa::ActiveObjectMap::AOMElement_ref semaphore =
      _activeObjectMap->isDestroying(oid);

    if (!semaphore.is_null()) {
      TIDThr::Synchronized synchro(*semaphore);
      try {
        semaphore->wait();
      } catch (const TIDThr::SystemException& e) {}
    }

    _activeObjectMap->put(oid, p_servant, uniqueId);
    // Servant delegate creation
    TIDorb::core::poa::ServantDelegate* delegate = new ServantDelegate(this, oid.get_object_id());
    p_servant->set_delegate(delegate);
  }
};


/**
* Create an object reference for a repository Id using a user defined object Id.
* @param oid The object Id
* @param intf The repository Id
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
CORBA::Object_ptr TIDorb::core::poa::POAImpl::create_reference_with_id
  (const TIDorb::core::poa::OID& oid, const char* intf)
throw (PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if (_IdAssignmentPolicy->value() == PortableServer::SYSTEM_ID) {

    TIDorb::core::poa::SystemOID* s_oid = TIDorb::core::poa::SystemOID::fromOID(oid);

    if(s_oid == NULL)
      throw CORBA::BAD_PARAM("Invalid System OID", 0, CORBA::COMPLETED_NO);
 
    if (_LifespanPolicy->value() == PortableServer::TRANSIENT) {
      if((s_oid->getPOAId() != _poa_id) ||
      (_currentOID->getSerial() < s_oid->getSerial())) {
        delete s_oid;
        throw CORBA::BAD_PARAM();
      }
    } else { // PERSISTENT
      if((s_oid->getPOAId() == _poa_id) &&
      (_currentOID->getSerial() < s_oid->getSerial())) {
        delete s_oid;
        throw CORBA::BAD_PARAM();
      }
    }

    return create_reference_with_id_aux(s_oid, intf);      //consume s_oid
  }

  return create_reference_with_id_aux(new OID(oid), intf); //consume new OID
};

/**
* Creates a new ObjectKey with the given oid. <p>
* If the Lifespan policy is TRANSIENT, the poa_id is set to remember that the
* key is only valid in the current poa context.
*/
TIDorb::core::poa::POAKey* TIDorb::core::poa::POAImpl::create_key(TIDorb::core::poa::OID* oid)
{

  if(_LifespanPolicy->value() == PortableServer::TRANSIENT)
    return new POAKey(this, _poa_id, oid);
  else
    return new POAKey(this, 0, oid);

};

/**
* Create an object reference for a repository Id using a user defined object Id.
* @param oid The object Id
* @param intf The repository Id
*/
CORBA::Object_ptr
  TIDorb::core::poa::POAImpl::create_reference_with_id_aux(TIDorb::core::poa::OID* oid,
                                                           const char* intf)
{
  TIDorb::core::poa::POAKey* poakey = create_key(oid);

  TIDorb::core::iop::VectorTaggedComponent poaComponents;


  if (_referencePolicies->getPolicies()->getSize() > 0) {
    size_t num_components = 1;
    poaComponents.resize(num_components);    
    poaComponents[0] = _referencePolicies;
  }

  TIDorb::core::iop::IOR* ior =
    orb->getCommunicationManager()->getExternalLayer()->createIOR(intf, 
                                     poakey, poaComponents);
  return TIDorb::core::ObjectImpl::fromIOR(orb, ior);
};

/**
* Get the servant of a given object Id.
* @param oid The object Id.
* @exception PortableServer::POA::ObjectNotActive
*            If object is not active yet.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
PortableServer::Servant TIDorb::core::poa::POAImpl::id_to_servant(const TIDorb::core::poa::OID& oid)
throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if ((_ServantRetentionPolicy->value() != PortableServer::RETAIN) &&
  (_RequestProcessingPolicy->value() != PortableServer::USE_DEFAULT_SERVANT)) {
    throw PortableServer::POA::WrongPolicy();
  }
//   if ((_ServantRetentionPolicy->value() == PortableServer::RETAIN) &&
//   (_activeObjectMap->isActive(oid))) {
//     return _activeObjectMap->get(oid);
//   }
  if (_ServantRetentionPolicy->value() == PortableServer::RETAIN){
    PortableServer::Servant servant = _activeObjectMap->get_if_Active(oid);
    if (servant)
      return servant;
  }
  if ((_RequestProcessingPolicy->value() == PortableServer::USE_DEFAULT_SERVANT) &&
  (_servant != NULL)) {
    // Default servant invocation (oid == current request oid ??)

    TIDorb::portable::ServantDelegate* aux_delegate=_servant->get_delegate();
    //jagd 
    //TIDorb::core::poa::ServantDelegate* delegate=dynamic_cast<TIDorb::core::poa::ServantDelegate*>(aux_delegate);
    TIDorb::core::poa::ServantDelegate* delegate=(TIDorb::core::poa::ServantDelegate*)(aux_delegate);
    delegate->setObjectId(oid.get_object_id());
    return _servant;
  }
  throw PortableServer::POA::ObjectNotActive();
};

/**
* Finds the servant for an object Id. Executes all needed activations,
* according to the policies of this POA.
* @param oid The object Id.
* @param operation The name of the operation invoked (if any).
* @param cookieHolder The CookieHolder (only if ServantLocator is needed).
* @param servantLocatorUsed Returns true if ServantLocator has been used.
* @return The servant.
* @exception PortableServer::POA::ObjectNotActive
*            If it tries to use a inactive object.
* @exception PortableServer::POA::ObjectAlreadyActive
*            If it tries to activate an active object.
* @exception PortableServer::ForwardRequest
*            It can be thrown by incarnate().
*/
PortableServer::Servant TIDorb::core::poa::POAImpl::find_servant(const TIDorb::core::poa::OID& oid,
const char* operation, PortableServer::ServantLocator::Cookie* cookie, CORBA::Boolean* servantLocatorUsed)
  throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::ObjectAlreadyActive,
         PortableServer::ForwardRequest, CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);
//MLG
  *servantLocatorUsed = false;
//EMLG  
  PortableServer::Servant servant = NULL;

  try {

    switch (_ServantRetentionPolicy->value()) {
    case PortableServer::RETAIN:
  
      switch (_RequestProcessingPolicy->value()) {
      case PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY: {
        // RETAIN && USE_ACTIVE_OBJECT_MAP_ONLY
        servant = id_to_servant(oid);
        break;
      }
      case PortableServer::USE_DEFAULT_SERVANT: {
        // RETAIN && USE_DEFAULT_SERVANT
        servant = id_to_servant(oid);
        break;
      }
      case PortableServer::USE_SERVANT_MANAGER: {
        // RETAIN && USE_SERVANT_MANAGER
        if (_servant_manager == NULL) {
          throw CORBA::OBJ_ADAPTER();
        }
        try {
          // try to get id from the Active Object Map
          servant = id_to_servant(oid);
        } catch (const PortableServer::POA::ObjectNotActive &e) {
          // try to use Servant Activator (incarnate)
          
          PortableServer::ServantActivator_var activator =
            PortableServer::ServantActivator::_narrow(_servant_manager);
          
          servant = activator->incarnate(oid.get_object_id(), this);
          if (!servant) {
            throw CORBA::OBJ_ADAPTER();
          }
          try {
            // try to activate servant created by Servant Activator
            activate_object_with_id(oid, servant);
          } catch (const PortableServer::POA::ServantAlreadyActive &e2) {
            throw CORBA::OBJ_ADAPTER();
          }
        }
      }
      }; // switch
      break;
       
    case PortableServer::NON_RETAIN: 

      switch (_RequestProcessingPolicy->value()) {
      case PortableServer::USE_SERVANT_MANAGER: {
        // NON_RETAIN && USE_SERVANT_MANAGER
        if (_servant_manager == NULL) {
          throw CORBA::OBJ_ADAPTER("No  ServantManager setted");
        }
        // try to use Servant Locator (preinvoke)
        PortableServer::ServantLocator* locator =
          PortableServer::ServantLocator::_narrow(_servant_manager);
        
        if (isSingleThread()) {
          TIDThr::Synchronized synchro(*single_thread_recursive_mutex);
          servant = locator->preinvoke(oid.get_object_id(), this, (char*)operation, *cookie);
          
        } else {
          servant = locator->preinvoke(oid.get_object_id(), this, (char*)operation, *cookie);
        }
        
        if(!servant)
          throw CORBA::OBJ_ADAPTER("ServantLocator::preinvoke returns NULL");
          
        *servantLocatorUsed = true;
        // Servant delegate creation (oid == current request oid ??)
        TIDorb::core::poa::ServantDelegate* delegate = 
          new ServantDelegate(this, oid.get_object_id());
        servant->set_delegate(delegate);
        break;
      }
      case PortableServer::USE_DEFAULT_SERVANT: {
          // NON_RETAIN && USE_DEFAULT_SERVANT
        if (_servant == NULL) {
          throw CORBA::OBJ_ADAPTER();
        }
        // return Default Servant
        servant = _servant;
        // Default servant invocation (oid == current request oid ??)
        TIDorb::portable::ServantDelegate* aux_delegate=servant->get_delegate();
        //jagd 
        // TIDorb::core::poa::ServantDelegate* delegate=
        //   dynamic_cast<TIDorb::core::poa::ServantDelegate*>(aux_delegate);
        TIDorb::core::poa::ServantDelegate* delegate = 
          (TIDorb::core::poa::ServantDelegate*)(aux_delegate);
        delegate->setObjectId(oid.get_object_id());
      }
      case PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY: {
        // NON_RETAIN && USE_ACTIVE_OBJECT_MAP_ONLY
        // should never happen: WRONG POLICY !!
      }
      }; // switch
      
    }; // switch
    
  
  
//     // RETAIN && USE_ACTIVE_OBJECT_MAP_ONLY
//     if ( (_ServantRetentionPolicy->value() == PortableServer::RETAIN) && 
//         (_RequestProcessingPolicy->value() == PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY)) {
//       servant = id_to_servant(oid);
//     }

//     // RETAIN && USE_DEFAULT_SERVANT
//     if ((_ServantRetentionPolicy->value() == PortableServer::RETAIN) &&
//     (_RequestProcessingPolicy->value() == PortableServer::USE_DEFAULT_SERVANT)) {
//       servant = id_to_servant(oid);
//     }

//     // RETAIN && USE_SERVANT_MANAGER
//     if ((_ServantRetentionPolicy->value() == PortableServer::RETAIN) &&
//     (_RequestProcessingPolicy->value() == PortableServer::USE_SERVANT_MANAGER)) {
//       if (_servant_manager == NULL) {
//         throw CORBA::OBJ_ADAPTER();
//       }
//       try {
//         // try to get id from the Active Object Map
//         servant = id_to_servant(oid);
//       } catch (const PortableServer::POA::ObjectNotActive &e) {
//         // try to use Servant Activator (incarnate)

//         PortableServer::ServantActivator_var activator =
//           PortableServer::ServantActivator::_narrow(_servant_manager);

//         servant = activator->incarnate(oid.get_object_id(), this);
//         if (!servant) {
//           throw CORBA::OBJ_ADAPTER();
//         }
//         try {
//           // try to activate servant created by Servant Activator
//           activate_object_with_id(oid, servant);
//         } catch (const PortableServer::POA::ServantAlreadyActive &e2) {
//           throw CORBA::OBJ_ADAPTER();
//         }
//       }
//     }


//     // NON_RETAIN && USE_SERVANT_MANAGER
//     if ((_ServantRetentionPolicy->value() == PortableServer::NON_RETAIN) &&
//     (_RequestProcessingPolicy->value() == PortableServer::USE_SERVANT_MANAGER)) {
//       if (_servant_manager == NULL) {
//         throw CORBA::OBJ_ADAPTER("No  ServantManager setted");
//       }
//       // try to use Servant Locator (preinvoke)
//       PortableServer::ServantLocator* locator =
//         PortableServer::ServantLocator::_narrow(_servant_manager);

//       if (isSingleThread()) {
//         TIDThr::Synchronized synchro(*single_thread_recursive_mutex);
//         servant = locator->preinvoke(oid.get_object_id(), this, (char*)operation, *cookie);

//       } else {
//         servant = locator->preinvoke(oid.get_object_id(), this, (char*)operation, *cookie);
//       }

//       if(!servant)
//         throw CORBA::OBJ_ADAPTER("ServantLocator::preinvoke returns NULL");

//       *servantLocatorUsed = true;
//       // Servant delegate creation (oid == current request oid ??)
//       TIDorb::core::poa::ServantDelegate* delegate = new ServantDelegate(this, oid.get_object_id());
//       servant->set_delegate(delegate);
//     }

//     // NON_RETAIN && USE_DEFAULT_SERVANT
//     if ((_ServantRetentionPolicy->value() == PortableServer::NON_RETAIN) &&
//     (_RequestProcessingPolicy->value() == PortableServer::USE_DEFAULT_SERVANT)) {
//       if (_servant == NULL) {
//         throw CORBA::OBJ_ADAPTER();
//       }
//       // return Default Servant
//       servant = _servant;
//       // Default servant invocation (oid == current request oid ??)
//       TIDorb::portable::ServantDelegate* aux_delegate=servant->get_delegate();
//       //jagd 
//       //TIDorb::core::poa::ServantDelegate* delegate=dynamic_cast<TIDorb::core::poa::ServantDelegate*>(aux_delegate);
//       TIDorb::core::poa::ServantDelegate* delegate=(TIDorb::core::poa::ServantDelegate*)(aux_delegate);
//       delegate->setObjectId(oid.get_object_id());
//     }


  } catch (const PortableServer::POA::WrongPolicy &dummy) {
    // should never happen !!
  }

  return servant;
};

/**
* Call postinvoke. This method is only used if there was
* a before servant invocation.
* @param oid The object Id.
* @param operation The name of the operation invoked.
* @param cookieHolder The CookieHolder
* @param servant The servant.
* @return The servant.
*/
void TIDorb::core::poa::POAImpl::call_postinvoke(const PortableServer::ObjectId& oid, const char* operation,
PortableServer::ServantLocator::Cookie cookie, PortableServer::Servant servant) {

  TIDThr::Synchronized synchro(recursive_mutex);

  if ((_ServantRetentionPolicy->value() == PortableServer::NON_RETAIN) &&
  (_RequestProcessingPolicy->value() == PortableServer::USE_SERVANT_MANAGER)) {
    if (_servant_manager == NULL) {
      throw CORBA::OBJ_ADAPTER();
    }
    PortableServer::ServantLocator* locator =
      PortableServer::ServantLocator::_narrow(_servant_manager);
    if (isSingleThread()) {
      TIDThr::Synchronized synchro(*single_thread_recursive_mutex);
      locator->postinvoke(oid, this, (char*)operation, cookie, servant);

    } else {
      locator->postinvoke(oid, this, (char*)operation, cookie, servant);
    }
  }
};

/**
* @return True if POA's ThreadPolicy is SINGLE_THREAD.
*/
CORBA::Boolean TIDorb::core::poa::POAImpl::isSingleThread() {
  return (_ThreadPolicy->value() == PortableServer::SINGLE_THREAD_MODEL);
};

/**
* Adds a new user for this oid. This means that there is
* one more active request executing some request with this
* object Id.
* @param oid The object Id.
* @exception PortableServer::POA::ObjectNotActive
*            If object is not active yet.
*/
void TIDorb::core::poa::POAImpl::addUser(const TIDorb::core::poa::OID& oid)
throw (PortableServer::POA::ObjectNotActive) {

  TIDThr::Synchronized synchro(recursive_mutex);

  if (_ServantRetentionPolicy->value() == PortableServer::RETAIN) {

//     if (_activeObjectMap->isActive(oid)) {
//       _activeObjectMap->addUser(oid);
//     }
    _activeObjectMap->addUser_if_Active(oid);
  }

  _completion.beginRequest();
  _POAManager->beginRequest();
};

/**
* Removes a user for this oid.
* @param oid The object Id.
* @exception PortableServer::POA::ObjectNotActive
*            If object is not active yet.
* @see addUser.
*/
void TIDorb::core::poa::POAImpl::removeUser(const TIDorb::core::poa::OID& oid)
throw (PortableServer::POA::ObjectNotActive, CORBA::SystemException) {
  // Decrease # of users of OID
  _completion.endRequest();

  {
    TIDThr::Synchronized synchro(recursive_mutex);
    
    if (_destroyed)
      return;

    _POAManager->endRequest();

    if (_destroying) {
      CORBA::Boolean PoaNotInUse = _completion.getActiveRequests() <= 0;
      if (PoaNotInUse) {
        trueDestroy();
      }

    } else {
      tryToRemoveObject(oid);
    }
  }
}

/**
* Etherealizes all objects in this POA.
*/
void TIDorb::core::poa::POAImpl::etherealizeAllObjects() {

  TIDThr::Synchronized synchro(recursive_mutex);

  if (_ServantRetentionPolicy->value() == PortableServer::RETAIN) {

    if ((_RequestProcessingPolicy->value() == PortableServer::USE_SERVANT_MANAGER) &&
        (_servant_manager != NULL)) {
      // call etherealize
      PortableServer::ServantActivator* activator =
        PortableServer::ServantActivator::_narrow(_servant_manager);

      ActiveObjectMap::AOMTable::iterator iterador;
      iterador = _activeObjectMap->_table.begin();

      while (iterador != _activeObjectMap->_table.end()) {

        TIDorb::core::poa::OID oid = (*iterador).first;
        try {
          PortableServer::Servant servant = _activeObjectMap->get_and_remove(oid); 
          if (isSingleThread()) {
            TIDThr::Synchronized synchro(*single_thread_recursive_mutex);
            activator->etherealize(oid.get_object_id(), this, servant, false, 
                                   _activeObjectMap->contains(servant));
          } else {
            activator->etherealize(oid.get_object_id(), this, servant, false, 
                                   _activeObjectMap->contains(servant));
          }
        } catch (const CORBA::Exception &e) {
          // should never happen
          if (orb->trace != NULL){
            TIDorb::util::StringBuffer msg;
            msg << toString() << "should never happend!! ";
            orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data(),e);
          }
        }
        iterador = _activeObjectMap->_table.begin();
      }
    }
  }
  else
  {
    //nothing to do
  }
};


/**
* Create a POA as a children of this POA.
* @param adapter_name The POA name of the new POA.
* @param a_POAManager The POA manager which the new POA belongs to.
* @param policies The policies to be used in the new POA.
* @exception PortableServer::POA::AdapterAlreadyExists
*            When this POA has alreay a children named adapter_name.
* @exception PortableServer::POA::InvalidPolicy
*            When policies are invalid.
*/
PortableServer::POA_ptr 
TIDorb::core::poa::POAImpl::create_POA(const char* adapter_name, 
                                       PortableServer::POAManager_ptr a_POAManager,
                                       const ::CORBA::PolicyList& policies)
throw (PortableServer::POA::AdapterAlreadyExists, PortableServer::POA::InvalidPolicy,
       CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);
  
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if((adapter_name == NULL))
  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  if (find_children(adapter_name) != NULL) {
    throw PortableServer::POA::AdapterAlreadyExists();
  }

  TIDorb::core::poa::POAManagerImpl* POAmgr = NULL;
  if (a_POAManager == NULL) {
    POAmgr = new POAManagerImpl(orb);
  } else {
    //jagd
    //POAmgr = dynamic_cast<TIDorb::core::poa::POAManagerImpl*>(a_POAManager);
    POAmgr = (TIDorb::core::poa::POAManagerImpl*)(a_POAManager);
  }

  TIDorb::core::poa::POAImpl* newPOA =
    new POAImpl(orb, adapter_name, (PortableServer::POAManager*)POAmgr, policies, this);

  if(_destroying){
    newPOA->destroy(false,false);
  } else {
    _children.push_back(newPOA);
  }


  if(a_POAManager == NULL) { // recently created

    TIDorb::core::PolicyContext* context = NULL;
    context = newPOA->getPolicyContext();

    if (context != NULL){
      CORBA::Policy_ptr queuePolicy = NULL;
      queuePolicy = context->getPolicy(Messaging::QUEUE_ORDER_POLICY_TYPE);

      if(queuePolicy != NULL) 
        POAmgr->setQueueOrderPolicy(queuePolicy);  
    }
    

  }


  return newPOA;
};

/**
* Find a POA among this POA's children.
* @param adapter_name The POA name.
* @param activate_it If it is true find_POA() will try to activate
*                    the POA in case it can't find it.
* @exception PortableServer::POA::AdapterNonExistent
*            When this POA has not a children named adapter_name.
*/
//jagd
//PortableServer::POA_ptr TIDorb::core::poa::POAImpl::find_POA(const char* adapter_name,CORBA::Boolean activate_it)
TIDorb::core::poa::POAImpl  * TIDorb::core::poa::POAImpl::find_POA(const char* adapter_name,CORBA::Boolean activate_it)
throw (PortableServer::POA::AdapterNonExistent, CORBA::SystemException)
{
  {
    TIDThr::Synchronized synchro(recursive_mutex);

    if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);
  }

  if(adapter_name == NULL)
  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  //jagd
  //PortableServer::POA_ptr poa = find_children(adapter_name);
  TIDorb::core::poa::POAImpl * poa = find_children(adapter_name);

  if (poa == NULL) {
    if ((activate_it) && (_activator != NULL)) {
      try {
        _activator->unknown_adapter(this, (char*)adapter_name);
      } catch (const CORBA::Exception &e) {
        //MCPG - Pendiente
        //throw CORBA::CORBA::OBJ_ADAPTER(e.getMessage());
      }
      poa = find_children(adapter_name);  // see if _activator has created the POA
      if (poa == NULL) {
        // AdapterActivator failed!
        throw PortableServer::POA::AdapterNonExistent();
      }

      if(_destroying)
      poa->destroy(false,false);

      return poa;

    } else {
      // AdapterActivator does not exist!
      throw PortableServer::POA::AdapterNonExistent();
    }
  } else
  return poa;
};

/**
* Destroy the POA.
* @param etherealize_objects  If true, it must etherealize all objects.
* @param wait_for_completion Wait-for-completion flag.
*/
void TIDorb::core::poa::POAImpl::destroy(CORBA::Boolean etherealize_objects, CORBA::Boolean wait_for_completion)
throw(CORBA::SystemException)
{
  {
    TIDThr::Synchronized synchro(recursive_mutex);

    if (!_destroying) {
      for (size_t i=0; i<_children.size(); i++) {
        PortableServer::POA_ptr poa = (PortableServer::POA_ptr)_children[i];
        poa->destroy(etherealize_objects, wait_for_completion);
      }
      _etherealize = etherealize_objects;
    }
    _destroying = true;
  }

  if (wait_for_completion) {

    // Wait until there are no executing requests
    if (_completion.conditionToWait()) {
      _completion.waitForCompletion();
    } else {
      throw CORBA::BAD_INV_ORDER("Can't waitForCompletion!");
    }

    {
      TIDThr::Synchronized synchro(recursive_mutex);

      if (!_destroyed) {
        // POA has not been destroyed yet -> destroy it now
        trueDestroy();
      }
    }

  } else {

    {
      TIDThr::Synchronized synchro(recursive_mutex);

      CORBA::Boolean PoaNotInUse = _completion.getActiveRequests() <= 0;
      if (PoaNotInUse) {
        trueDestroy();
      }
    }

  }
};

/* ************ Policy creation ************* */

PortableServer::ThreadPolicy_ptr TIDorb::core::poa::POAImpl::create_thread_policy(
PortableServer::ThreadPolicyValue value) throw(CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return new TIDorb::core::poa::policies::ThreadPolicyImpl(value);
};

PortableServer::LifespanPolicy_ptr TIDorb::core::poa::POAImpl::create_lifespan_policy(
PortableServer::LifespanPolicyValue value) throw(CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return new TIDorb::core::poa::policies::LifespanPolicyImpl(value);
};

PortableServer::IdUniquenessPolicy_ptr TIDorb::core::poa::POAImpl::create_id_uniqueness_policy(
PortableServer::IdUniquenessPolicyValue value) throw(CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return new TIDorb::core::poa::policies::IdUniquenessPolicyImpl(value);
};

PortableServer::IdAssignmentPolicy_ptr TIDorb::core::poa::POAImpl::create_id_assignment_policy(
PortableServer::IdAssignmentPolicyValue value) throw(CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return new TIDorb::core::poa::policies::IdAssignmentPolicyImpl(value);
};

PortableServer::ImplicitActivationPolicy_ptr TIDorb::core::poa::POAImpl::create_implicit_activation_policy(
PortableServer::ImplicitActivationPolicyValue value) throw(CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return new TIDorb::core::poa::policies::ImplicitActivationPolicyImpl(value);
};

PortableServer::ServantRetentionPolicy_ptr TIDorb::core::poa::POAImpl::create_servant_retention_policy(
PortableServer::ServantRetentionPolicyValue value) throw(CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return new TIDorb::core::poa::policies::ServantRetentionPolicyImpl(value);
};

PortableServer::RequestProcessingPolicy_ptr TIDorb::core::poa::POAImpl::create_request_processing_policy(
PortableServer::RequestProcessingPolicyValue value) throw(CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return new TIDorb::core::poa::policies::RequestProcessingPolicyImpl(value);
};

/* ************ Accessors ************* */
CORBA::ULongLong TIDorb::core::poa::POAImpl::the_id()
{
  return _poa_id;
};

char* TIDorb::core::poa::POAImpl::the_name() throw(CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return (char*)_name;
};

PortableServer::POA_ptr TIDorb::core::poa::POAImpl::the_parent() throw(CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return (PortableServer::POA*)_parent;
};

const TIDorb::core::poa::POAPath& TIDorb::core::poa::POAImpl::the_path() const throw(CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return _path;
};

PortableServer::POAList* TIDorb::core::poa::POAImpl::the_children() throw(CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  PortableServer::POAList* childrenArray = new PortableServer::POAList();
  childrenArray->length(_children.size());

  for (size_t i = 0; i < _children.size(); i++)
  //jagd
    //(*childrenArray)[i] = PortableServer::POA::_duplicate(_children[i]);
    {
      _children[i]->_add_ref();
      (*childrenArray)[i] = _children[i];
    }
  return childrenArray;
};

PortableServer::POAManager_ptr TIDorb::core::poa::POAImpl::the_POAManager() throw(CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return PortableServer::POAManager::_duplicate(_POAManager);
};

PortableServer::AdapterActivator_ptr TIDorb::core::poa::POAImpl::the_activator() throw(CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  return PortableServer::AdapterActivator::_duplicate(_activator);
};

void TIDorb::core::poa::POAImpl::the_activator(PortableServer::AdapterActivator_ptr the_activator) throw(CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if(the_activator == NULL)
  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  _activator = PortableServer::AdapterActivator::_duplicate(the_activator);
};

PortableServer::ServantManager_ptr TIDorb::core::poa::POAImpl::get_servant_manager()
throw (PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if (_RequestProcessingPolicy->value() != PortableServer::USE_SERVANT_MANAGER) {
    throw PortableServer::POA::WrongPolicy();
  }

  return PortableServer::ServantManager::_duplicate(_servant_manager);
};

void TIDorb::core::poa::POAImpl::set_servant_manager(PortableServer::ServantManager_ptr imgr)
throw (PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if(imgr == NULL)
  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  if (_RequestProcessingPolicy->value() != PortableServer::USE_SERVANT_MANAGER) {
    throw PortableServer::POA::WrongPolicy();
  }
  if (_ServantRetentionPolicy->value() == PortableServer::RETAIN) {
    PortableServer::ServantActivator_ptr aux_activator= dynamic_cast<PortableServer::ServantActivator_ptr>(imgr);
    if (!(aux_activator)) {
      throw CORBA::OBJ_ADAPTER();
    }
  }
  if (_ServantRetentionPolicy->value() == PortableServer::NON_RETAIN) {
    PortableServer::ServantLocator_ptr aux_locator= dynamic_cast<PortableServer::ServantLocator_ptr>(imgr);
    if (!(aux_locator)) {
      throw CORBA::OBJ_ADAPTER();
    }
  }
  if (_servant_manager != NULL) {
    throw CORBA::BAD_INV_ORDER();
  }
  _servant_manager = PortableServer::ServantManager::_duplicate(imgr);
};

PortableServer::Servant TIDorb::core::poa::POAImpl::get_servant()
throw (PortableServer::POA::NoServant,PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if (_RequestProcessingPolicy->value() != PortableServer::USE_DEFAULT_SERVANT) {
    throw PortableServer::POA::WrongPolicy();
  }

  if (_servant == NULL) {
    throw PortableServer::POA::NoServant();
  }
  return _servant;
};

void TIDorb::core::poa::POAImpl::set_servant(PortableServer::Servant p_servant)
throw (PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if(p_servant == NULL)
  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  if (_RequestProcessingPolicy->value() != PortableServer::USE_DEFAULT_SERVANT) {
    throw PortableServer::POA::WrongPolicy();
  }
  _servant = p_servant;

  _servant->_add_ref();

  // Servant delegate creation (oid == null ?)

  if(_servant->get_delegate() == NULL) {
    PortableServer::ObjectId oid;
    TIDorb::core::poa::ServantDelegate* delegate= new ServantDelegate(this,oid);
    _servant->set_delegate(delegate);
  }
}

/* ************ Activation ************* */

/**
* Activate an object in this POA.
* @param p_servant Servant.
* @return Returns an ORB generated object Id.
* @exception PortableServer::POA::ServantAlreadyActive
*            If p_servant is already active in this POA.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
PortableServer::ObjectId_ptr
  TIDorb::core::poa::POAImpl::activate_object(PortableServer::Servant p_servant)
throw (PortableServer::POA::ServantAlreadyActive,PortableServer::POA::WrongPolicy,
       CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  PortableServer::ObjectId_ptr result = NULL;

  if (_destroyed) {
    throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);
  }

  if(p_servant == NULL) {
    throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);
  }

  if (_IdAssignmentPolicy->value() != PortableServer::SYSTEM_ID) {
    throw PortableServer::POA::WrongPolicy();
  }

  TIDorb::core::poa::OID* id = nextOID();
  try {
    activate_object_with_id(*id, p_servant);
  } catch (PortableServer::POA::ObjectAlreadyActive) {
    // this should never happen
    delete id;
    throw CORBA::INTERNAL("ObjectAlreadyActive thrown with SYSTEM_ID policy!!");
  } catch (PortableServer::POA::ServantAlreadyActive) {
    delete id;
    throw;
  } catch (PortableServer::POA::WrongPolicy) {
    delete id;
    throw;
  }

  result = id->toByteArray();
  delete id;

  return result;
}

/**
* Activate an object in this POA.
* @param id User created object Id.
* @param p_servant Servant.
* @exception PortableServer::POA::ObjectAlreadyActive
*            If id is already active in this POA.
* @exception PortableServer::POA::ServantAlreadyActive
*            If p_servant is already active in this POA.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
void TIDorb::core::poa::POAImpl::activate_object_with_id(const PortableServer::ObjectId& id,
                                                         PortableServer::Servant p_servant)
throw (PortableServer::POA::ServantAlreadyActive,PortableServer::POA::ObjectAlreadyActive,
       PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  //      if ((id == NULL) || (p_servant == NULL))
  //  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  if(id.length() == 0)
  throw CORBA::BAD_PARAM("Invalid id length: 0", 0,CORBA::COMPLETED_NO);

  //PRA
  //TIDorb::core::poa::OID* aux_oid=new OID(id);
  TIDorb::core::poa::OID aux_oid(id);
  //EPRA
  activate_object_with_id(aux_oid, p_servant);

}

/**
* Deactivate an object in this POA.
* @param oid The object Id.
* @exception PortableServer::POA::ObjectNotActive
*            If id is not active in this POA.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
void TIDorb::core::poa::POAImpl::deactivate_object(const PortableServer::ObjectId& oid)
throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  //if (oid == NULL)
  //  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  if (oid.length() == 0)
  throw CORBA::BAD_PARAM("Invalid oid length: 0", 0,CORBA::COMPLETED_NO);

  if (_ServantRetentionPolicy->value() != PortableServer::RETAIN) {
    throw PortableServer::POA::WrongPolicy();
  }

  //PRA
  //TIDorb::core::poa::OID* tid_oid = new OID(oid);
  TIDorb::core::poa::OID tid_oid(oid);
  //EPRA

  {
    TIDThr::Synchronized synchro(recursive_mutex);

    if (_activeObjectMap->isDestroying(tid_oid) == NULL) {
      _activeObjectMap->destroy(tid_oid);
      tryToRemoveObject(tid_oid);
    }
  }
}

/* ************ Id conversion ************* */

/**
* Create an object reference for a repository Id (using a new object Id).
* @param intf The repository Id
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
CORBA::Object_ptr TIDorb::core::poa::POAImpl::create_reference(const char* intf)
throw (PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  //if(intf == NULL)
  //  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  if(strlen(intf) == 0)
  throw CORBA::BAD_PARAM("Invalid id length: 0", 0,CORBA::COMPLETED_NO);

  if (_IdAssignmentPolicy->value() != PortableServer::SYSTEM_ID) {
    throw PortableServer::POA::WrongPolicy();
  }

  TIDorb::core::poa::OID* newOid = nextOID();

  return create_reference_with_id_aux(newOid, intf);
}

/**
* Create an object reference for a repository Id using a user defined object Id.
* @param oid The object Id
* @param intf The repository Id
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
CORBA::Object_ptr
  TIDorb::core::poa::POAImpl::create_reference_with_id(const PortableServer::ObjectId& oid,
                                                       const char* intf)
throw (CORBA::SystemException)
{

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  //if((oid==NULL) || (intf == NULL))
  //  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  if(oid.length() == 0)
  throw CORBA::BAD_PARAM("Invalid oid length: 0", 0,CORBA::COMPLETED_NO);

  if(strlen(intf) == 0)
  throw CORBA::BAD_PARAM("Invalid intf length: 0", 0,CORBA::COMPLETED_NO);

  OID aux_oid(oid);
  return create_reference_with_id(aux_oid, intf);
}

/**
* Get the object Id of a given servant.
* @param p_servant The servant.
* @exception PortableServer::POA::ServantNotActive
*            If p_servant is not active yet.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
PortableServer::ObjectId_ptr
  TIDorb::core::poa::POAImpl::servant_to_id(PortableServer::Servant p_servant)
  throw (PortableServer::POA::ServantNotActive,PortableServer::POA::WrongPolicy,
         CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  //f(p_servant == NULL)
  //  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  CORBA::Boolean hasRetain = (_ServantRetentionPolicy->value() == PortableServer::RETAIN);
  CORBA::Boolean hasUniqueId = (_IdUniquenessPolicy->value() == PortableServer::UNIQUE_ID);
  CORBA::Boolean hasImplicitActivation = (_ImplicitActivationPolicy->value() == PortableServer::IMPLICIT_ACTIVATION);
  CORBA::Boolean hasDefaultServant = (_RequestProcessingPolicy->value() == PortableServer::USE_DEFAULT_SERVANT);

 
  if(!(hasDefaultServant || (hasRetain&&(hasUniqueId||hasImplicitActivation))))
  throw PortableServer::POA::WrongPolicy();

  if(hasRetain) {
    // 1st behaviour
    if ((hasUniqueId) && (_activeObjectMap->contains(p_servant))) {
      const OID& oid = _activeObjectMap->get(p_servant);
      return oid.toByteArray();
    }
  }
    // 2nd behaviour
  if (hasImplicitActivation) {
     if ((! _activeObjectMap->contains(p_servant)) || (!hasUniqueId))
     {
       TIDorb::core::poa::OID* newOid = nextOID();
       try {
         activate_object_with_id(*newOid, p_servant);
         PortableServer::ObjectId_ptr oid = newOid->toByteArray();
         delete newOid;
         return oid;

       } catch (const PortableServer::POA::ObjectAlreadyActive &e) {
         delete newOid;
         throw CORBA::INTERNAL(); // this should never happen
       } catch (const PortableServer::POA::ServantAlreadyActive &e) {
         delete newOid;
         throw CORBA::INTERNAL(); // this should never happen
       }
    }
  }

  // 3rd behaviour
  if (hasDefaultServant && (_servant == p_servant)) {
    try {
      CORBA::Object_var object = orb->init_POACurrent();
      PortableServer::Current_var current = PortableServer::Current::_narrow(object);
      PortableServer::Servant servant = current->get_servant();
      if (servant == _servant) {
        // operation invoked on the default servant
        //return orb->init_POACurrent()->get_object_id();
        return current->get_object_id();
      }
    } catch(const CORBA::Exception &e) {
      throw PortableServer::POA::ServantNotActive();
    }
  }

  // 4th behaviour
  throw PortableServer::POA::ServantNotActive();
};

/**
* Get the object reference of a given servant.
* @param p_servant The servant.
* @exception PortableServer::POA::ServantNotActive
*            If p_servant is not active yet.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
CORBA::Object_ptr TIDorb::core::poa::POAImpl::servant_to_reference(PortableServer::Servant p_servant)
throw (PortableServer::POA::ServantNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if(p_servant == NULL)
    throw CORBA::BAD_PARAM("Null servant reference", 0,CORBA::COMPLETED_NO);

  CORBA::Boolean hasRetain = (_ServantRetentionPolicy->value() == PortableServer::RETAIN);
  CORBA::Boolean hasUniqueId = (_IdUniquenessPolicy->value() == PortableServer::UNIQUE_ID);
  CORBA::Boolean hasImplicitActivation = (_ImplicitActivationPolicy->value() == PortableServer::IMPLICIT_ACTIVATION);

  if (!(hasRetain && (hasUniqueId || hasImplicitActivation))) {
    throw PortableServer::POA::WrongPolicy();
  }

  // 1st behaviour
  if (hasUniqueId) {
    if(_activeObjectMap->contains(p_servant))
    {
      const TIDorb::core::poa::OID& oid = _activeObjectMap->get(p_servant);
      return create_reference_with_id(oid.get_object_id(), getRepositoryId(p_servant));
    }
  }

  // 2nd behaviour
  if (hasImplicitActivation) {

    if ((!_activeObjectMap->contains(p_servant)) || (!hasUniqueId)) {
      TIDorb::core::poa::OID* newOid = nextOID();
      try {
          activate_object_with_id(*newOid, p_servant);
          CORBA::Object_ptr ref = create_reference_with_id(*newOid, getRepositoryId(p_servant));
          delete newOid;
          return ref;
      } catch (const PortableServer::POA::ObjectAlreadyActive &e) {
        delete newOid;
        throw CORBA::INTERNAL(); // this should never happen
      } catch (const PortableServer::POA::ServantAlreadyActive &e) {
        delete newOid;
        throw CORBA::INTERNAL(); // this should never happen
      }
    }
  }
  // 3rd behaviour
  try {
    CORBA::Object_var object = orb->init_POACurrent();
    PortableServer::Current_var current = PortableServer::Current::_narrow(object);
    PortableServer::Servant servant = current->get_servant();

    if (servant == _servant) {
      // operation invoked on the default servant
      //PortableServer::ObjectId_ptr currentOID = orb->init_POACurrent()->get_object_id();
      const PortableServer::ObjectId_ptr currentOID = current->get_object_id();
      return create_reference_with_id(*currentOID, getRepositoryId(servant));
    }
  } catch(const CORBA::Exception &e) {
    throw PortableServer::POA::ServantNotActive();
  }

  // 4th behaviour
  throw PortableServer::POA::ServantNotActive();
};

/**
* Get the servant of a given object reference.
* @param reference The object reference.
* @exception PortableServer::POA::ObjectNotActive
*            If object is not active yet.
* @exception PortableServer::POA::WrongAdapter
*            If the object reference does not belong to this POA.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
* // pra@tid.es - MIOP extensions
* @exception CORBA::INV_OBJREF
*            If reference is a Group Object reference.
* // end MIOP extensions
*
*/
PortableServer::Servant TIDorb::core::poa::POAImpl::reference_to_servant(CORBA::Object_ptr reference)
throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::WrongAdapter,PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if (reference == NULL)
  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  PortableServer::ObjectId_var oid = reference_to_id(reference);
  return id_to_servant(*oid);
};

/**
* Get the object Id of a given object reference.
* @param reference The object reference.
* @exception PortableServer::POA::WrongAdapter
*            If the object reference does not belong to this POA.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
* // pra@tid.es - MIOP extensions
* @exception CORBA::INV_OBJREF
*            If reference is a Group Object reference.
* // end MIOP extensions
*
*/
PortableServer::ObjectId_ptr TIDorb::core::poa::POAImpl::reference_to_id(CORBA::Object_ptr reference)
throw (PortableServer::POA::WrongAdapter,PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  if(reference == NULL)
  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  try {
    TIDorb::portable::Stub* stub = dynamic_cast <TIDorb::portable::Stub*> (reference);

    if(!stub)
      throw PortableServer::POA::WrongAdapter();

    TIDorb::core::ObjectDelegateImpl* delegate =
      dynamic_cast<TIDorb::core::ObjectDelegateImpl*> (stub->_get_delegate());

    TIDorb::core::poa::POAKey* poaKey = delegate->getReference()->object_key()->get_key();

    if (!poaKey->samePOA(_poa_id, _path)) {
      throw PortableServer::POA::WrongAdapter();
    }

    const TIDorb::core::poa::OID& oid = poaKey->getOID();
    return oid.toByteArray();

  } catch (const CORBA::Exception &e) {
    throw PortableServer::POA::WrongAdapter();
  }
};

/**
* Get the servant of a given object Id.
* @param oid The object Id.
* @exception PortableServer::POA::ObjectNotActive
*            If object is not active yet.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
PortableServer::Servant TIDorb::core::poa::POAImpl::id_to_servant(const PortableServer::ObjectId& oid)
throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);

  //if(oid == NULL)
  //  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  if(oid.length() == 0)
  throw CORBA::BAD_PARAM("Invalid oid length: 0", 0,CORBA::COMPLETED_NO);

  //PRA
  //TIDorb::core::poa::OID* aux_oid = new OID(oid);
  TIDorb::core::poa::OID aux_oid(oid);
  //EPRA
  return id_to_servant(aux_oid);
};

/**
* Get the object reference of a given object Id.
* @param oid The object Id.
* @exception PortableServer::POA::ObjectNotActive
*            If object is not active yet.
* @exception PortableServer::POA::WrongPolicy
*            If POA policies do not allow this operation.
*/
CORBA::Object_ptr TIDorb::core::poa::POAImpl::id_to_reference(const PortableServer::ObjectId& oid)
throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException)
{
  if (_destroyed)
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);
  //if(oid == NULL)
  //  throw CORBA::BAD_PARAM("Null reference", 0,CORBA::COMPLETED_NO);

  if(oid.length() == 0)
  throw CORBA::BAD_PARAM("Invalid oid length: 0", 0,CORBA::COMPLETED_NO);

  //PRA
  //TIDorb::core::poa::OID* tid_oid = new OID(oid);
  TIDorb::core::poa::OID tid_oid(oid);
  //EPRA

  if (_ServantRetentionPolicy->value() != PortableServer::RETAIN) {
    throw PortableServer::POA::WrongPolicy();
  }
  if (!_activeObjectMap->isActive(tid_oid)) {
    throw PortableServer::POA::ObjectNotActive();
  }
  PortableServer::Servant servant = _activeObjectMap->get(tid_oid);
  return create_reference_with_id(tid_oid, getRepositoryId(servant));
};

/* ************ TIDorb methods ************* */

/**
* @return True if POA's BidirectionalPolicy is BOTH.
*/
CORBA::Boolean TIDorb::core::poa::POAImpl::isBidirectional() {

  TIDThr::Synchronized synchro(recursive_mutex);

//   CORBA::Policy_ptr policy = 
//     _referencePolicies->getPolicies()->getPolicy(BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE); 
        
//   TIDorb::core::BidirectionalPolicyImpl* bidirectional_policy = 
//     dynamic_cast <TIDorb::core::BidirectionalPolicyImpl*> (policy);

//   if(bidirectional_policy == NULL) {
//     return false;
//   } else {
//     return (bidirectional_policy->value() == BiDirPolicy::BOTH);
//   }
  return (_BidirectionalPolicy->value() == BiDirPolicy::BOTH);
};

/**
* @return True if this POA has an AdapterActivator.
*/
CORBA::Boolean TIDorb::core::poa::POAImpl::hasAdapterActivator() {
  TIDThr::Synchronized synchro(recursive_mutex);

  return _activator != NULL;
};


/**
* Creates the root POA.
* This method should only be called by TIDorb::core::TIDORB::resolve_initial_references()
* @param orb The ORB.
* @see TIDorb::core::TIDORB#resolve_initial_references
*/
TIDorb::core::poa::POAImpl* TIDorb::core::poa::POAImpl::createRootPOA(TIDorb::core::TIDORB* orb)
{
  TIDorb::core::poa::POAImpl* rootPOA = NULL;
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
    TIDorb::core::poa::POAManagerImpl* mgr = new POAManagerImpl(orb);
    rootPOA = new POAImpl(orb, "rootPOA" , mgr, policies, NULL);


  } catch (const PortableServer::POA::InvalidPolicy& e) {
    // this can never happen
  }

  return rootPOA;
};

void TIDorb::core::poa::POAImpl::destroy() throw(CORBA::SystemException)
{
  throw CORBA::BAD_OPERATION();
};

CORBA::OctetSeq* TIDorb::core::poa::POAImpl::id() throw (CORBA::SystemException)
{
  CORBA::Octet* buffer = new CORBA::Octet[TIDorb::core::cdr::CDR::ULONGLONG_SIZE];
  *((CORBA::ULongLong *) buffer) = _poa_id;

  return new CORBA::OctetSeq(TIDorb::core::cdr::CDR::ULONGLONG_SIZE,
  TIDorb::core::cdr::CDR::ULONGLONG_SIZE,
  buffer, true);
};

/**
* Destroys the POA.
*/
void TIDorb::core::poa::POAImpl::trueDestroy()
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if (_destroyed)
    return;

  if (_etherealize) {
    // Etherealize all objects
    etherealizeAllObjects();
  } /*PRA else {
    // init activeObjectMap (perhaps this is not necessary)
    _activeObjectMap = new ActiveObjectMap();
  } EPRA*/

  // remove POA from parent
  if (_parent != NULL) {
    TIDorb::core::poa::POAImpl* aux_parent  =
      dynamic_cast<TIDorb::core::poa::POAImpl*>((PortableServer::POA *)_parent);

    //vector<PortableServer::POA_var>::iterator iterador;
    vector<TIDorb::core::poa::POAImpl *>::iterator iterador;
    iterador = aux_parent->_children.begin();
    while (iterador != aux_parent->_children.end()){
      PortableServer::POA* poa_aux=*iterador;
      if (strcmp(poa_aux->the_name(),_name)==0){
        aux_parent->_children.erase(iterador);
        iterador=aux_parent->_children.begin();
      }
      else
        iterador++;
    }
//     vector<PortableServer::POA_var>::iterator iterador;
//     for (iterador=aux_parent->_children.begin(); iterador!= aux_parent->_children.end();iterador++) {
//       //if (strcmp(aux_parent->_children.operator[](iterator)->the_name(),_name)==0)
//       PortableServer::POA* poa_aux=*iterador;
//       if (strcmp(poa_aux->the_name(),_name)==0)
//         aux_parent->_children.erase(iterador);
//       //aux_parent->_children.removeElement(this);
//     }
  }

  // Remove RootPOA from ORB services; it could be recreated (bug #771)
  if (strcmp(_name,"rootPOA") == 0) {
    TIDorb::core::ORBServices& services = 
      const_cast<TIDorb::core::ORBServices&>(orb->services());
    services.remove_service(TIDorb::core::ORBServices::ROOT_POA_ID);
    orb->remove_POA();
  }

  _destroyed = true;
  _POAManager->removePOA(this);
  _POAManager = NULL;



  delete _activeObjectMap; // delete move from ~POAImpl()

};

/**
* @return The current System Object Id.
*/
const TIDorb::core::poa::SystemOID* TIDorb::core::poa::POAImpl::current_OID()
{
  return _currentOID;
};

/**
* Generates the next oid.
* @return The next Object Id.
*/
TIDorb::core::poa::OID* TIDorb::core::poa::POAImpl::nextOID()
{
  TIDorb::core::poa::SystemOID*  aux = _currentOID;
  _currentOID = _currentOID->next();
  return aux;
};


const char* TIDorb::core::poa::POAImpl::toString()
{
  if(_poa_string_name == NULL){

    TIDorb::core::poa::POAKey* key = create_key(NULL);
    TIDorb::util::StringBuffer  os;
    CORBA::String_var path =  key->getPOAPath();
    os << "POA " << path;
    _poa_string_name = CORBA::string_dup(os.str().data());
    delete key;
  }

  return _poa_string_name;
}

/**
* @param self The servant.
* @return Return the RootPOA of the default ORB
*/

PortableServer::POA_ptr
TIDorb::core::poa::POAImpl::default_POA()
{
  CORBA::ORB_var orb = TIDorb::core::TIDORB::default_ORB();

  CORBA::Object_var ref = orb->resolve_initial_references("RootPOA");

  return PortableServer::POA::_narrow(ref);
};
