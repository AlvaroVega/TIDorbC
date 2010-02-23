/////////////////////////////////////////////////////////////////////////
//
// File:        ServantDelegate.h
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

#ifndef _TIDORB_CORE_POA_ServantDelegate_H_
#define _TIDORB_CORE_POA_ServantDelegate_H_

/**
 * Delegate class for servants.
 * @version 1.0
*/
namespace TIDorb {
namespace core {
namespace poa {
class ServantDelegate : public TIDorb::portable::ServantDelegate
{
private:    

    CORBA::ORB_ptr           _orb_deleg;
    PortableServer::POA_var  _poa;
    PortableServer::ObjectId _oid;
    //PRA
    TIDorb::portable::ORB*   _orb;
    //EPRA

public:

    static const CORBA::RepositoryIdSeq_ptr ids;
    
  /**
    * Constructor.
    * @param poa The POA.
    * @param oid The Object Id.
    */
    ServantDelegate(PortableServer::POA_ptr poa, 
                    const PortableServer::ObjectId& oid)
      throw (TIDThr::SystemException);
    
    ServantDelegate(const ServantDelegate& oid)
      throw (TIDThr::SystemException);
    
    ~ServantDelegate() throw (TIDThr::SystemException);
    
    /**
    * @param self The servant.
    * @return The ORB.
    */
    //PRA
    const CORBA::ORB_ptr         orb(PortableServer::ServantBase* self);
    const TIDorb::portable::ORB* orb();
    //EPRA
    
    /**
    * @param self The servant.
    * @return Return an object reference for this servant.
    */
    CORBA::Object_ptr this_object(PortableServer::ServantBase* self);
    
    /**
    * @param self The servant.
    * @return This POA.
    */
    PortableServer::POA_ptr poa(PortableServer::ServantBase* self);
  
    /**
    * @param self The servant.
    * @return Return the Object Id.
    */
    const PortableServer::ObjectId& object_id(PortableServer::ServantBase* self);
    
    /**
    * @param self The servant.
    * @return Return the default POA.
    */
    PortableServer::POA_ptr default_POA(PortableServer::ServantBase* self);
                  
    /**
    * @param self The servant.
    * @param repository_id The Repository Id.
    * @return Return true if the servant is an object with this Repository Id.
    */
    CORBA::Boolean is_a(PortableServer::ServantBase* self, 
                        const char* repository_id);
    
    const CORBA::RepositoryIdSeq_ptr 
      all_interfaces(PortableServer::ServantBase* self,
                     PortableServer::POA_ptr poa, 
                     const PortableServer::ObjectId& objectId);
                     
    /**
    * Sets object Id (useful for default servants).
    * @param oid The Object Id.
    */
    void setObjectId(const PortableServer::ObjectId& oid);
    
    CORBA::InterfaceDef_ptr get_interface(PortableServer::ServantBase* self);
  
    CORBA::Boolean non_existent(PortableServer::ServantBase* self);
    
    static CORBA::RepositoryIdSeq_ptr init_ids();
};

}
}
}

#endif

