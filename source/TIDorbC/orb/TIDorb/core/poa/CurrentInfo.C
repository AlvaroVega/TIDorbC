/////////////////////////////////////////////////////////////////////////
//
// File:        CurrentInfo.C
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

#include "TIDorb/core/poa/CurrentInfo.h"

TIDorb::core::poa::CurrentInfo::CurrentInfo(TIDorb::core::poa::POAImpl* current_poa, 
                                            const TIDorb::core::poa::OID& current_oid)
{
	_current_poa = current_poa;
	_current_poa->_add_ref();
	_current_oid = current_oid;
  _request_in_poa = false;
	_servant = NULL;
};

TIDorb::core::poa::CurrentInfo::~CurrentInfo() {
  CORBA::release(_current_poa);
  if(_servant)
    _servant->_remove_ref();
};
  
CORBA::Boolean TIDorb::core::poa::CurrentInfo::is_request_in_POA()
{
	return _request_in_poa;
};
  
void TIDorb::core::poa::CurrentInfo::set_request_in_POA(CORBA::Boolean val)
{
	_request_in_poa = val;
};
  
void TIDorb::core::poa::CurrentInfo::setServant(PortableServer::ServantBase* servant) {
  if(_servant)
    _servant->_remove_ref();

  _servant = servant;

  if(_servant)
    _servant->_add_ref();
}
