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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
