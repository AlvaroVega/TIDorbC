/////////////////////////////////////////////////////////////////////////
//
// File:        LocalLocateRequest.C
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

#include "TIDorb/core/poa/LocalLocateRequest.h"
#include "TIDorb/core/util/exceptions.h"



TIDorb::core::poa::LocalLocateRequest::LocalLocateRequest
  (TIDorb::core::poa::POAKey* poakey,
   TIDorb::core::poa::POAImpl* root_poa)
    : AbstractLocalRequest(poakey, root_poa),
      _locate_name(NULL),
      _exception(NULL),
      forward_obj(NULL),
      found(false)
{
}

TIDorb::core::poa::LocalLocateRequest::~LocalLocateRequest()
  throw(TIDThr::SystemException)
{
  delete _exception;
  delete forward_obj;
}




CORBA::SystemException* TIDorb::core::poa::LocalLocateRequest::exception()
{
  return _exception;
}




void TIDorb::core::poa::LocalLocateRequest::exception(const CORBA::SystemException& excp)
{
  _exception = TIDorb::core::util::exceptions::SystemExceptionEncoder::copy(excp);
}




CORBA::Boolean TIDorb::core::poa::LocalLocateRequest::has_exception()
{
  return _exception != NULL;
}




CORBA::Boolean TIDorb::core::poa::LocalLocateRequest::is_completed()
{
  return state.is_completed();
}




void TIDorb::core::poa::LocalLocateRequest::set_completed()
{
  state.set_completed();
}




void TIDorb::core::poa::LocalLocateRequest::wait_for_completion(CORBA::ULong how_long)
{
  try {
    state.wait_for_completion(how_long);
  } catch (TIDThr::InterruptedException &ie) {
      throw CORBA::INTERNAL("Unexpected Interrupted Exception");
  } catch (TIDorb::core::util::OnlyOneThreadCanWait &only) {
      throw CORBA::INTERNAL("Only one Thread can wait in the request lock.");
  }
}




CORBA::Boolean TIDorb::core::poa::LocalLocateRequest::is_forwarded()
{
    return forward_obj != NULL;
}




void TIDorb::core::poa::LocalLocateRequest::set_forward(CORBA::Object_ptr obj)
{
    forward_obj = obj;
}




CORBA::Object_ptr TIDorb::core::poa::LocalLocateRequest::get_forward()
{
  return forward_obj;
}




const char* TIDorb::core::poa::LocalLocateRequest::toString() const
{
  if(_locate_name == NULL) {
          /*StringBuffer name = new StringBuffer();
          name.append("LocalLocateRequest to ");
          name.append(getPOAKey().toString());
          _locate_name = name.toString();*/
      TIDorb::util::StringBuffer  os;
      os << "LocalLocateRequest to " << getPOAKey()->toString();

      ((LocalLocateRequest*)this)->_locate_name = CORBA::string_dup(os.str().data());
  }

  return _locate_name;
}




void TIDorb::core::poa::LocalLocateRequest::returnError(const CORBA::SystemException& e,
                                                        TIDorb::util::Trace* trace)
{
  try {

    if (trace != NULL){
      TIDorb::util::StringBuffer  msg;
      msg << toString() << " error returned " ;
      trace->print(TIDorb::util::TR_DEBUG, msg.str().data(),
                   TIDorb::core::util::CORBAException(e));
    }

    exception(e);
    set_completed();

  } catch (...) {
  }
}


void TIDorb::core::poa::LocalLocateRequest::dispatch(TIDorb::core::poa::ExecThread * exec,
                                                     TIDorb::core::poa::POAImpl * poa)
{
  exec->execute(this,poa);
}

// void* TIDorb::core::poa::LocalLocateRequest::_impl()
// {
// 	return this;
// }

// const char* TIDorb::core::poa::LocalLocateRequest::_typeid()
// {
// 	//return CORBA::string_dup("LocalLocateRequest");
// 	return "LocalLocateRequest";
// }
