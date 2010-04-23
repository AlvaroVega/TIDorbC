/////////////////////////////////////////////////////////////////////////
//
// File:        POAImpl.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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

#include "TIDorb/core/poa.h"

#ifndef _TIDORB_CORE_POA_POAImpl_H_
#define _TIDORB_CORE_POA_POAImpl_H_

/**
 * Implementation of POA.
 * @version 1.0
 */
#include "TIDThr.h"

#include <vector>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::vector;
#endif

namespace TIDorb {
namespace core {
namespace poa {

class POAImpl : public virtual PortableServer::POA,
                public virtual CORBA::LocalObject

{
protected:
  bool _destroyed;

  TIDThr::RecursiveMutex recursive_mutex;
  TIDThr::RecursiveMutexHandle single_thread_recursive_mutex;

  CORBA::String_var _name;
  char* _poa_string_name;
  CORBA::ULongLong _poa_id;
  vector<POAImpl *> _children;
  PortableServer::POA_var _parent;
  TIDorb::core::poa::POAPath _path;
  TIDorb::core::poa::POAManagerImpl* _POAManager;
  TIDorb::core::poa::CompletionWaiter _completion;
  bool _destroying;
  bool _etherealize;
  PortableServer::AdapterActivator_var _activator;
  PortableServer::ServantManager_var _servant_manager;
  PortableServer::Servant _servant;
  TIDorb::core::poa::ActiveObjectMap* _activeObjectMap;
  TIDorb::core::poa::SystemOID* _currentOID;
  TIDorb::core::poa::policies::IdAssignmentPolicyImpl* _IdAssignmentPolicy;
  TIDorb::core::poa::policies::IdUniquenessPolicyImpl* _IdUniquenessPolicy;
  TIDorb::core::poa::policies::ImplicitActivationPolicyImpl* _ImplicitActivationPolicy;
  TIDorb::core::poa::policies::LifespanPolicyImpl* _LifespanPolicy;
  TIDorb::core::poa::policies::RequestProcessingPolicyImpl* _RequestProcessingPolicy;
  TIDorb::core::poa::policies::ServantRetentionPolicyImpl* _ServantRetentionPolicy;
  TIDorb::core::poa::policies::ThreadPolicyImpl* _ThreadPolicy;
  TIDorb::core::BidirectionalPolicyImpl* _BidirectionalPolicy;

  TIDorb::core::messaging::PoliciesComponent_ref _referencePolicies;

/* ************ Private methods ************* */

    /**
    * Removes an object if there are no executing request using it.
    * @param oid The object Id.
    */
    void tryToRemoveObject(const TIDorb::core::poa::OID& oid)
        throw (PortableServer::POA::ObjectNotActive);

    /**
    * Removes an object.
    * @param oid The object Id.
    */
    void trueRemoveObject(const TIDorb::core::poa::OID& oid)
        throw (PortableServer::POA::ObjectNotActive );

    /**
    * Find a POA among the POA's children.
    * @param poa_name The POA name.
    */
    POAImpl* find_children(const char* poa_name);

    /**
    * Returns the repository Id of a servant.
    * @param servant The servant.
    * @return The repository Id.
    */
    const char* getRepositoryId(PortableServer::Servant servant);

public:
    /**
     * Returns the policy context
     */
    TIDorb::core::PolicyContext* getPolicyContext();


protected:

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
  void activate_object_with_id(const TIDorb::core::poa::OID& oid,PortableServer::Servant p_servant)
  throw (PortableServer::POA::ServantAlreadyActive,
         PortableServer::POA::ObjectAlreadyActive,
         PortableServer::POA::WrongPolicy,CORBA::SystemException);

      /**
    * Create an object reference for a repository Id using a user defined object Id.
    * @param oid The object Id
    * @param intf The repository Id
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    */
  CORBA::Object_ptr create_reference_with_id(const TIDorb::core::poa::OID& oid,
                                             const char* intf)
    throw (PortableServer::POA::WrongPolicy,
           CORBA::SystemException);

    /**
    * Creates a new ObjectKey with the given oid. <p>
    * If the Lifespan policy is TRANSIENT, the poa_id is set to remember that the
    * key is only valid in the current poa context.
    */
  TIDorb::core::poa::POAKey* create_key(TIDorb::core::poa::OID* oid);

  /**
    * Create an object reference for a repository Id using a user defined object Id.
    * @param oid The object Id
    * @param intf The repository Id
    */
    CORBA::Object_ptr
      create_reference_with_id_aux(TIDorb::core::poa::OID* oid,
                                   const char* intf);

  /**
    * Get the servant of a given object Id.
    * @param oid The object Id.
    * @exception PortableServer::POA::ObjectNotActive
    *            If object is not active yet.
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    */
    PortableServer::Servant id_to_servant(const TIDorb::core::poa::OID& oid)
        throw (PortableServer::POA::ObjectNotActive,
               PortableServer::POA::WrongPolicy,
               CORBA::SystemException);

public:
  TIDorb::core::TIDORB* orb;

  /**
    * Constructor.
    * @param orb The ORB.
    * @param adapter_name The POA name.
    * @param a_POAManager The POA manager which this POA belongs to.
    * @param policies The policies to be used in the POA.
    * @param parent The parent POA of this POA.
    * @exception PortableServer::POA::InvalidPolicy When policies are invalid.
    */
  POAImpl(TIDorb::core::TIDORB* the_orb,
                const char* adapter_name,
                PortableServer::POAManager_ptr a_POAManager,
                const CORBA::PolicyList& policies,
                PortableServer::POA_ptr parent)
        throw (PortableServer::POA::InvalidPolicy, CORBA::SystemException);

  /* Destructor */
  virtual ~POAImpl();


  TIDThr::RecursiveMutex* get_single_thread_recursive_mutex()
  {
    return single_thread_recursive_mutex;
  }

  /**
    * Removes a user for this oid.
    * @param oid The object Id.
    * @exception PortableServer::POA::ObjectNotActive
    *            If object is not active yet.
    * @see addUser.
    */
  void removeUser(const TIDorb::core::poa::OID& oid)
      throw (PortableServer::POA::ObjectNotActive, CORBA::SystemException);

  /**
  * Etherealizes all objects in this POA manager.
  */
  void etherealizeAllObjects();

  /**
    * Call postinvoke. This method is only used if there was
    * a preinvoke before servant invocation.
    * @param oid The object Id.
    * @param operation The name of the operation invoked.
    * @param cookie The Cookie
    * @param servant The servant.
    * @return The servant.
    */
    void call_postinvoke(const PortableServer::ObjectId& oid,
          const char* operation,PortableServer::ServantLocator::Cookie cookie,
          PortableServer::Servant servant);

    /**
    * @return True if POA's ThreadPolicy is SINGLE_THREAD.
    */
    bool isSingleThread();

  /**
    * Adds a new user for this oid. This means that there is
    * one more active request executing some request with this
    * object Id.
    * @param oid The object Id.
    * @exception PortableServer::POA::ObjectNotActive
    *            If object is not active yet.
    */
    void addUser(const TIDorb::core::poa::OID& oid)
      throw (PortableServer::POA::ObjectNotActive);

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
    PortableServer::Servant find_servant(
          const TIDorb::core::poa::OID& oid,
          const char* operation,
          PortableServer::ServantLocator::Cookie* cookie,
          CORBA::Boolean* servantLocatorUsed)
      throw (PortableServer::POA::ObjectNotActive,
            PortableServer::POA::ObjectAlreadyActive,
            PortableServer::ForwardRequest, CORBA::SystemException);

    /**
    * Create a POA as a children of this POA.
    * @param adapter_name The name of the new POA.
    * @param a_POAManager The POA manager which the new POA belongs to.
    * @param policies The policies to be used in the new POA.
    * @exception PortableServer::POA::AdapterAlreadyExists
    *            When this POA has alreay a children named adapter_name.
    * @exception PortableServer::POA::InvalidPolicy
    *            When policies are invalid.
    */
    PortableServer::POA_ptr create_POA(const char* adapter_name,
          PortableServer::POAManager_ptr a_POAManager,
                const ::CORBA::PolicyList& policies)
      throw (PortableServer::POA::AdapterAlreadyExists,
             PortableServer::POA::InvalidPolicy,
             CORBA::SystemException);

    /**
    * Find a POA among this POA's children.
    * @param adapter_name The POA name.
    * @param activate_it If it is true find_POA() will try to activate
    *                    the POA in case it can't find it.
    * @exception PortableServer::POA::AdapterNonExistent
    *            When this POA has not a children named adapter_name.
    */
    //jagd
    //PortableServer::POA_ptr find_POA(const char* adapter_name,CORBA::Boolean activate_it)
    POAImpl * find_POA(const char* adapter_name,CORBA::Boolean activate_it)
    throw (PortableServer::POA::AdapterNonExistent, CORBA::SystemException);


    /**
    * Destroy the POA.
    * @param etherealize_objects  If true, it must etherealize all objects.
    * @param wait_for_completion Wait-for-completion flag.
    */
    void destroy(CORBA::Boolean etherealize_objects, CORBA::Boolean wait_for_completion) throw(CORBA::SystemException);

    CORBA::OctetSeq* id() throw (CORBA::SystemException);

  /* ************ Policy creation ************* */

  PortableServer::ThreadPolicy_ptr create_thread_policy(
    PortableServer::ThreadPolicyValue value) throw(CORBA::SystemException);

  PortableServer::LifespanPolicy_ptr create_lifespan_policy(
    PortableServer::LifespanPolicyValue value) throw (CORBA::SystemException) ;

  PortableServer::IdUniquenessPolicy_ptr create_id_uniqueness_policy(
    PortableServer::IdUniquenessPolicyValue value) throw(CORBA::SystemException);

  PortableServer::IdAssignmentPolicy_ptr create_id_assignment_policy(
    PortableServer::IdAssignmentPolicyValue value) throw(CORBA::SystemException);

  PortableServer::ImplicitActivationPolicy_ptr create_implicit_activation_policy(
    PortableServer::ImplicitActivationPolicyValue value) throw(CORBA::SystemException);

  PortableServer::ServantRetentionPolicy_ptr create_servant_retention_policy(
    PortableServer::ServantRetentionPolicyValue value) throw(CORBA::SystemException);

  PortableServer::RequestProcessingPolicy_ptr create_request_processing_policy(
    PortableServer::RequestProcessingPolicyValue value) throw(CORBA::SystemException);

  /* ************ Accessors ************* */

  CORBA::ULongLong the_id();

  char* the_name() throw(CORBA::SystemException);

  PortableServer::POA_ptr the_parent() throw(CORBA::SystemException);

  const TIDorb::core::poa::POAPath& the_path() const throw(CORBA::SystemException);

  PortableServer::POAList* the_children() throw(CORBA::SystemException);

  PortableServer::POAManager_ptr the_POAManager() throw(CORBA::SystemException);

  PortableServer::AdapterActivator_ptr the_activator() throw(CORBA::SystemException);

  void the_activator(PortableServer::AdapterActivator_ptr the_activator) throw(CORBA::SystemException);

  PortableServer::ServantManager_ptr get_servant_manager()
    throw (PortableServer::POA::WrongPolicy,CORBA::SystemException);

  void set_servant_manager(PortableServer::ServantManager_ptr imgr)
    throw (PortableServer::POA::WrongPolicy,CORBA::SystemException);

  PortableServer::Servant get_servant()
    throw (PortableServer::POA::NoServant,PortableServer::POA::WrongPolicy,CORBA::SystemException);

  void set_servant(PortableServer::Servant p_servant)
    throw (PortableServer::POA::WrongPolicy,CORBA::SystemException);

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
    PortableServer::ObjectId_ptr activate_object(PortableServer::Servant p_servant)
    throw (PortableServer::POA::ServantAlreadyActive,PortableServer::POA::WrongPolicy,CORBA::SystemException);

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
    void activate_object_with_id(const PortableServer::ObjectId& id,PortableServer::Servant p_servant)
    throw (PortableServer::POA::ServantAlreadyActive,PortableServer::POA::ObjectAlreadyActive,
           PortableServer::POA::WrongPolicy,CORBA::SystemException);

    /**
    * Deactivate an object in this POA.
    * @param oid The object Id.
    * @exception PortableServer::POA::ObjectNotActive
    *            If id is not active in this POA.
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    */
    void deactivate_object(const PortableServer::ObjectId& oid)
    throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException);


  /* ************ Id conversion ************* */

    /**
    * Create an object reference for a repository Id (using a new object Id).
    * @param intf The repository Id
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    */
    CORBA::Object_ptr create_reference(const char* intf)
    throw (PortableServer::POA::WrongPolicy,CORBA::SystemException);

    /**
    * Create an object reference for a repository Id using a user defined object Id.
    * @param oid The object Id
    * @param intf The repository Id
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    */
    CORBA::Object_ptr create_reference_with_id(const PortableServer::ObjectId& oid,const char* intf)
    throw (CORBA::SystemException);

  /**
    * Get the object Id of a given servant.
    * @param p_servant The servant.
    * @exception PortableServer::POA::ServantNotActive
    *            If p_servant is not active yet.
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    */
    PortableServer::ObjectId_ptr servant_to_id(PortableServer::Servant p_servant)
    throw (PortableServer::POA::ServantNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException);

    /**
    * Get the object reference of a given servant.
    * @param p_servant The servant.
    * @exception PortableServer::POA::ServantNotActive
    *            If p_servant is not active yet.
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    */
    CORBA::Object_ptr servant_to_reference(PortableServer::Servant p_servant)
    throw (PortableServer::POA::ServantNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException);

    /**
    * Get the servant of a given object reference.
    * @param reference The object reference.
    * @exception PortableServer::POA::ObjectNotActive
    *            If object is not active yet.
    * @exception PortableServer::POA::WrongAdapter
    *            If the object reference does not belong to this POA.
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    * // MIOP extensions
    * @exception CORBA::INV_OBJREF
    *            If  reference is a Group Object reference.
    * // end MIOP extensions
    *
    */
    PortableServer::Servant reference_to_servant(CORBA::Object_ptr reference)
    throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::WrongAdapter,
           PortableServer::POA::WrongPolicy,CORBA::SystemException);

    /**
    * Get the object Id of a given object reference.
    * @param reference The object reference.
    * @exception PortableServer::POA::WrongAdapter
    *            If the object reference does not belong to this POA.
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    * // MIOP extensions
    * @exception CORBA::INV_OBJREF
    *            If reference is a Group Object reference.
    * // end MIOP extensions
    *
    */
    PortableServer::ObjectId_ptr reference_to_id(CORBA::Object_ptr reference)
    throw (PortableServer::POA::WrongAdapter,PortableServer::POA::WrongPolicy,CORBA::SystemException);

    /**
    * Get the servant of a given object Id.
    * @param oid The object Id.
    * @exception PortableServer::POA::ObjectNotActive
    *            If object is not active yet.
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    */
    PortableServer::Servant id_to_servant(const PortableServer::ObjectId& oid)
    throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException);

    /**
    * Get the object reference of a given object Id.
    * @param oid The object Id.
    * @exception PortableServer::POA::ObjectNotActive
    *            If object is not active yet.
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    */
    CORBA::Object_ptr id_to_reference(const PortableServer::ObjectId& oid)
    throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException);

  /* ************ TIDorb methods ************* */

    /**
    * @return True if POA's BidirectionalPolicy is BOTH.
    */
    bool isBidirectional();

    /**
    * @return True if this POA has an AdapterActivator.
    */
    bool hasAdapterActivator();

    /**
    * Creates the root POA.
    * This method should only be called by TIDorb::core::TIDORB::resolve_initial_references()
    * @param orb The ORB.
    * @see TIDorb::core::TIDORB#resolve_initial_references
    */
    static TIDorb::core::poa::POAImpl* createRootPOA(TIDorb::core::TIDORB* orb);

    void destroy() throw(CORBA::SystemException);

    /**
    * Destroys the POA manager.
    */
    void trueDestroy();

    /**
    * @return The current System Object Id.
    */
    const TIDorb::core::poa::SystemOID* current_OID();

    /**
    * Generates the next oid.
    * @return The next Object Id.
    */
    TIDorb::core::poa::OID* nextOID();

  /* ************ ObjectImpl ************* */

/*
    bool _is_a(const char* repositoryIdentifier);

    bool _is_equivalent(CORBA::Object_ptr other);
*/
    const char* toString();

  /*
   * Returns the RootPOA from the default ORB (inizialized with -ORBid0="")
   */

    static PortableServer::POA_ptr default_POA();

};

}//poa
}//core
}//TIDorb

#endif
