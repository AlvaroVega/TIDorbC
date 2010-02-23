/////////////////////////////////////////////////////////////////////////
//
// File:        MIOPRequest.C
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

#include "TIDorb/core/goa/MIOPRequest.h"


TIDorb::core::goa::MIOPRequest::MIOPRequest
  (TIDorb::core::poa::POAKey* poakey,
   TIDorb::core::poa::POAImpl* root_poa,
   TIDorb::core::comm::Connection* conn,
   TIDorb::core::ServerRequestImpl* req)
    : AbstractMIOPRequest(poakey, root_poa, conn),
      request(req), miop_name(NULL)
{
}



TIDorb::core::goa::MIOPRequest::~MIOPRequest()
{
  delete request;
  CORBA::string_free(miop_name);
}




TIDorb::core::ServerRequestImpl* TIDorb::core::goa::MIOPRequest::get_request() const
{
  return request;
}




const char* TIDorb::core::goa::MIOPRequest::toString() const
{
  if (miop_name == NULL) {
    TIDorb::util::StringBuffer buffer;
    buffer << "MIOPRequest id(" << request->get_id() << ") operation \""
           << request->operation() << "\" invoked in "
           << getPOAKey()->toString() << ends;

    ((MIOPRequest*) this)->miop_name = CORBA::string_dup(buffer.str().data());
  }

  return miop_name;
}




void TIDorb::core::goa::MIOPRequest::returnError(const CORBA::SystemException& e, 
                                                 TIDorb::util::Trace* trace)
{
  try {

    if (trace != NULL){
      TIDorb::util::StringBuffer  msg;
      msg << toString() << " error returned " << e.what();
      trace->print(TIDorb::util::TR_DEBUG, msg.str().data(),
                   TIDorb::core::util::CORBAException(e));
    }

    if (request->with_response()) {
      if (connection->is_open()) {
        if (trace != NULL)
          trace->print(TIDorb::util::TR_DEEP_DEBUG, "return exception to MIOPRequest ",
                       TIDorb::core::util::CORBAException(e));
        request->set_system_exception(e);
        connection->send_reply(request);
      }
    }
  } catch (const exception& e) {
    if (trace != NULL)
      trace->print(TIDorb::util::TR_DEEP_DEBUG, 
                   "Exception catched in MIOPRequest", e);
  }

}


void TIDorb::core::goa::MIOPRequest::dispatch(TIDorb::core::poa::ExecThread * exec,
                                              TIDorb::core::poa::POAImpl * poa)
{
  exec->execute(this,poa);
}


// void* TIDorb::core::goa::MIOPRequest::_impl()
// {
// 	return this;
// }

// const char* TIDorb::core::goa::MIOPRequest::_typeid()
// {
// 	//return CORBA::string_dup("MIOPRequest");
// 	return "MIOPRequest";
// }
