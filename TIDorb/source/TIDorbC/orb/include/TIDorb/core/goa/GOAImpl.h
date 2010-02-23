//////////////////////////////////////////////////////////////////////////////////
//
// File:        GOAImpl.h
// Description: GOA implementation
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////


#include "TIDorb/core/poa.h"

#ifndef _TIDORB_CORE_GOA_GOAImpl_H_
#define _TIDORB_CORE_GOA_GOAImpl_H_

/**
 * Implementation of GOA.
 * @version 1.0
*/
#include "TIDThr.h"
#include "PortableGroup.h"


#ifdef __SUNPRO_LIBRARY_IOSTREAM
#define __STD_ISTREAM__
#define __STD_OSTREAM__
#endif

#include <map>

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#undef __STD_ISTREAM__
#undef __STD_OSTREAM__
#endif

#include <string>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
  using std::string;
#endif


namespace TIDorb {
namespace core {
namespace goa {

class GOAImpl : public virtual PortableGroup::GOA,
                public virtual TIDorb::core::poa::POAImpl

{
  public:

    typedef map<string, PortableGroup::IDs>                GroupObjectIDsTable;
    typedef map<TIDorb::core::poa::OID, CORBA::Object_ptr> GroupReferencesTable;


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
    GOAImpl(TIDorb::core::TIDORB* the_orb,
            const char* adapter_name,
            PortableServer::POAManager_ptr a_POAManager,
            const CORBA::PolicyList& policies,
            PortableServer::POA_ptr parent)
      throw (PortableServer::POA::InvalidPolicy, CORBA::SystemException);


    /* Destructor */
    ~GOAImpl();


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
    PortableServer::POA_ptr create_POA(const char* adapter_name,
                                       PortableServer::POAManager_ptr a_POAManager,
                                       const ::CORBA::PolicyList& policies)
      throw (PortableServer::POA::AdapterAlreadyExists,
             PortableServer::POA::InvalidPolicy,
             CORBA::SystemException);


    /**
    * Creates the root POA as a Group Object Adapter.
    * This method should only be called by TIDorb::core::TIDORB::resolve_initial_references()
    * @param orb The ORB.
    * @see TIDorb::core::TIDORB#resolve_initial_references
    */
    static TIDorb::core::poa::POAImpl* createRootPOA(TIDorb::core::TIDORB* orb);


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
    * Get the object reference of a given object Id.
    * @param oid The object Id.
    * @exception PortableServer::POA::ObjectNotActive
    *            If object is not active yet.
    * @exception PortableServer::POA::WrongPolicy
    *            If POA policies do not allow this operation.
    */
    CORBA::Object_ptr id_to_reference(const PortableServer::ObjectId& oid)
    throw (PortableServer::POA::ObjectNotActive,PortableServer::POA::WrongPolicy,CORBA::SystemException);


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


    //Operations, Constants & Attributes Declaration


    PortableServer::ObjectId* create_id_for_reference(CORBA::Object_ptr the_ref);

    PortableGroup::IDs* reference_to_ids(CORBA::Object_ptr the_ref);

    void associate_reference_with_id(CORBA::Object_ptr the_ref, const PortableServer::ObjectId& oid);

    void disassociate_reference_with_id(CORBA::Object_ptr the_ref, const PortableServer::ObjectId& oid);


    /**
     * Returns UIPMC profile of the reference
     */
    const TIDorb::core::comm::miop::ProfileMIOP* get_group_object_profile(CORBA::Object_ptr the_ref);

    /**
     * Returns group object id from the UIPMC profile of the reference
     */
    const char* get_group_object_id(CORBA::Object_ptr the_ref);

    /**
     * Adds to the POAKeyList those POAKeys corresponding to ObjectIds associated to group_object_id
     * in this GOA
     */
    void group_object_POAKeys(const string& group_object_id, TIDorb::core::poa::POAKeyList& keys);

    /**
     * Returns the ObjectIds associated to group_object_id in this GOA
     */
    PortableGroup::IDs* group_object_id_to_IDs(const string& group_object_id);

  protected:
    GroupObjectIDsTable  _group_object_ids_table;
    GroupReferencesTable _group_references_table;

};
}//goa
}//core
}//TIDorb
#endif
