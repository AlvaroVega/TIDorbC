/////////////////////////////////////////////////////////////////////////
//
// File:        LocalRequest.C
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

#include "TIDorb/core/poa/LocalRequest.h"


TIDorb::core::poa::LocalRequest::LocalRequest(TIDorb::core::poa::POAKey* poakey, 
                                              TIDorb::core::poa::POAImpl* root_poa, 
                                              TIDorb::core::LocalServerRequest* req)
  : AbstractLocalRequest(poakey, root_poa),
    request(req),
    local_name(NULL)
{
}


TIDorb::core::poa::LocalRequest::~LocalRequest()
  throw(TIDThr::SystemException)
{
  delete request;

  if (local_name)
    CORBA::string_free(local_name);
}




TIDorb::core::LocalServerRequest* TIDorb::core::poa::LocalRequest::get_request()
{
  return request;
}




CORBA::Boolean TIDorb::core::poa::LocalRequest::is_completed()
{
  return state.is_completed();
}




void TIDorb::core::poa::LocalRequest::set_completed()
{
  state.set_completed();
}




void TIDorb::core::poa::LocalRequest::wait_for_completion(CORBA::ULong how_long)
{
  try {
    state.wait_for_completion(how_long);
  } catch (TIDThr::InterruptedException &ie) {
    throw CORBA::INTERNAL("Unexpected Interrupted Exception");
  } catch (TIDorb::core::util::OnlyOneThreadCanWait &only) {
    throw CORBA::INTERNAL("Only one Thread can wait in the request lock.");
  }
}




const char* TIDorb::core::poa::LocalRequest::toString() const
{
  if (local_name == NULL) {
    TIDorb::util::StringBuffer buffer;
    buffer << "LocalRequest operation \"" << request->operation()
           << "\" invoked in " << getPOAKey()->toString() << ends;

    ((LocalRequest*) this)->local_name = CORBA::string_dup(buffer.str().data());
  }

  return local_name;
}




void TIDorb::core::poa::LocalRequest::returnError(const CORBA::SystemException& e, 
                                                  TIDorb::util::Trace* trace)
{
  try {

    if (trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << toString() << " error returned " ;
      trace->print(TIDorb::util::TR_DEBUG,
                   msg.str().data(),
                   TIDorb::core::util::CORBAException(e));
    }

    request->set_system_exception(e);
    set_completed();

  } catch(CORBA::INTERNAL &ex) {

    if (trace != NULL){
      TIDorb::util::StringBuffer  msg;
      msg << " Local Request error: system exception has not be set. " ;
      trace->print(TIDorb::util::TR_DEBUG, msg.str().data(),
                   TIDorb::core::util::CORBAException(e));
    }

    try {
      request->set_system_exception(ex);
    } catch(...) {
      if (trace != NULL){
        TIDorb::util::StringBuffer  msg;
        msg << " Local Request error: CORBA::INTERNAL has not be set. " ;
        msg << " Never be reached !! " ;
        trace->print(TIDorb::util::TR_DEBUG, msg.str().data(),
                     TIDorb::core::util::CORBAException(e));
      }
    }

  }

}


void TIDorb::core::poa::LocalRequest::dispatch(TIDorb::core::poa::ExecThread * exec,
                                               TIDorb::core::poa::POAImpl * poa)
{
  exec->execute(this,poa);
}

// void* TIDorb::core::poa::LocalRequest::_impl()
// {
// 	return this;
// }

// const char* TIDorb::core::poa::LocalRequest::_typeid()
// {
// 	//return CORBA::string_dup("LocalRequest");
// 	return "LocalRequest";
// }
