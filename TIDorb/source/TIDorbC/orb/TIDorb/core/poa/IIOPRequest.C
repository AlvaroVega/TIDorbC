/////////////////////////////////////////////////////////////////////////
//
// File:        IIOPRequest.C
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

#include "TIDorb/core/poa/IIOPRequest.h"


#include "TIDorb/core/poa/ExecThread.h"



TIDorb::core::poa::IIOPRequest::IIOPRequest
  (TIDorb::core::poa::POAKey* poakey,
   TIDorb::core::poa::POAImpl* root_poa,
   TIDorb::core::comm::Connection* conn,
   TIDorb::core::ServerRequestImpl* req)
    : AbstractIIOPRequest(poakey, root_poa, conn),
      request(req),
      iiop_name(NULL)
{

}




TIDorb::core::poa::IIOPRequest::~IIOPRequest()
{

  if (request)
    delete request;

  if (iiop_name)
    CORBA::string_free(iiop_name);
}




TIDorb::core::ServerRequestImpl* TIDorb::core::poa::IIOPRequest::get_request() const
{
  return request;
}




const char* TIDorb::core::poa::IIOPRequest::toString() const
{
  if (iiop_name == NULL) {
    TIDorb::util::StringBuffer buffer;
    buffer << "IIOPRequest id(" << request->get_id() << ") operation \""
           << request->operation() << "\" invoked in "
           << getPOAKey()->toString() << ends;

    ((IIOPRequest*) this)->iiop_name = CORBA::string_dup(buffer.str().data());
  }

  return iiop_name;
}




void TIDorb::core::poa::IIOPRequest::returnError(const CORBA::SystemException& e, 
                                                 TIDorb::util::Trace* trace)
{
  try {

    if (trace != NULL){
      TIDorb::util::StringBuffer  msg;
      msg << toString() << " error returned ";
      trace->print(TIDorb::util::TR_DEBUG, msg.str().data(),
                   TIDorb::core::util::CORBAException(e));
    }

    if (request->with_response()) {
      if (connection->is_open()) {
        if (trace != NULL)
          trace->print(TIDorb::util::TR_DEEP_DEBUG, "return exception to IIOPRequest ",
                       TIDorb::core::util::CORBAException(e));
        request->set_system_exception(e);
        connection->send_reply(request);
      }
    }

  } catch (const exception &th) {
    if (trace != NULL)
      trace->print(TIDorb::util::TR_DEEP_DEBUG, 
                   "Exception catched in IIOPRequest", th);
  }
}


void TIDorb::core::poa::IIOPRequest::dispatch(TIDorb::core::poa::ExecThread * exec,
                                              TIDorb::core::poa::POAImpl * poa) 
{
  exec->execute(this,poa);
}


// void* TIDorb::core::poa::IIOPRequest::_impl()
// {
// 	return this;
// }

// const char* TIDorb::core::poa::IIOPRequest::_typeid()
// {
// 	//return CORBA::string_dup("IIOPRequest");
// 	return "IIOPRequest";
// }

