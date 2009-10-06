/////////////////////////////////////////////////////////////////////////
//
// File:        DynamicImplementationImpl.C
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

#include "CORBA.h"
#include "PortableServer.h"


CORBA::Object_ptr PortableServer::DynamicImplementation::_this()
{
  TIDorb::portable::ServantDelegate* delegate = get_delegate();
    
  // No delegate implies the Servant is not activated
  if(delegate== NULL)
  {
    PortableServer::POA_var poa = _default_POA();

    //jagd
    //if(CORBA::is_nil(poa))
    if(!(poa))
      throw PortableServer::POA::WrongPolicy();

    return poa->servant_to_reference(this);
  } else {
    return delegate->this_object(this);
  }
}
