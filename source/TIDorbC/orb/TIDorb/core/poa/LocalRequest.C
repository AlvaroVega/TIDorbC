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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
