/*/////////////////////////////////////////////////////////////////////////
//
// File:        ServantDelegate.h
// Description: 
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////*/

#include "CORBA.h"
#include "PortableServer.h"

#ifndef _TIDORB_PORTABLE_SERVANT_DELEGATE_H_
#define _TIDORB_PORTABLE_SERVANT_DELEGATE_H_

#include "TIDThr.h"

namespace TIDorb {
namespace portable {

class ServantDelegate : public virtual TIDThr::Monitor {
  public:
   virtual ~ServantDelegate() throw (TIDThr::SystemException) {}
   virtual PortableServer::POA_ptr default_POA(PortableServer::ServantBase* self) = 0;
   virtual CORBA::InterfaceDef_ptr get_interface(PortableServer::ServantBase* self) = 0;
   virtual CORBA::Boolean          is_a(PortableServer::ServantBase* self, 
                                        const char* logical_type_id) = 0;
   virtual CORBA::Boolean          non_existent(PortableServer::ServantBase* self) = 0;
   
   //PRA
   virtual const CORBA::ORB_ptr         orb(PortableServer::ServantBase* self) = 0;
   virtual const TIDorb::portable::ORB* orb() = 0;
   //EPRA   
   virtual CORBA::Object_ptr       this_object(PortableServer::ServantBase* self) = 0;
   
   virtual const CORBA::RepositoryIdSeq_ptr 
      all_interfaces(PortableServer::ServantBase* self,
                     PortableServer::POA_ptr poa, 
                     const PortableServer::ObjectId& objectId) = 0;
   
  protected:
    ServantDelegate() throw (TIDThr::SystemException) {}
  
};

}
}

#endif
