/////////////////////////////////////////////////////////////////////////
//
// File:        IIOPLocaleRequest.C
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

#include "TIDorb/core/poa/IIOPLocateRequest.h"

TIDorb::core::poa::IIOPLocateRequest::IIOPLocateRequest(TIDorb::core::poa::POAKey* poakey,
  TIDorb::core::poa::POAImpl* root_poa, TIDorb::core::comm::Connection* conn,
  TIDorb::core::comm::iiop::RequestId id, const TIDorb::core::comm::iiop::Version& ver)
  : AbstractIIOPRequest(poakey, root_poa, conn),
    _id(id),
    _version(ver),
    iiop_locate_name(NULL)
{

}




TIDorb::core::poa::IIOPLocateRequest::~IIOPLocateRequest()
{
}




TIDorb::core::comm::iiop::RequestId
TIDorb::core::poa::IIOPLocateRequest::get_id() const
{
  return _id;
}




const TIDorb::core::comm::iiop::Version&
TIDorb::core::poa::IIOPLocateRequest::get_version() const
{
  return _version;
}




const char* TIDorb::core::poa::IIOPLocateRequest::toString() const
{

  if(iiop_locate_name == NULL) {
     TIDorb::util::StringBuffer  os;

      os << "IIOPLocateRequest id(" << _id + ") " << getPOAKey()->toString();
      ((IIOPLocateRequest*)this)->iiop_locate_name = CORBA::string_dup(os.str().data());
  }

  return iiop_locate_name;
}




void TIDorb::core::poa::IIOPLocateRequest::returnError(const CORBA::SystemException& e,
                                                       TIDorb::util::Trace* trace)
{
  try {

    if (trace != NULL){
      TIDorb::util::StringBuffer  msg;
      msg << toString() << " error returned ";
      trace->print(TIDorb::util::TR_DEBUG, msg.str().data(),
                   TIDorb::core::util::CORBAException(e));
    }

    if (connection->is_open()) {
      connection->send_locate_reply(_version, _id, e);
    }

  } catch (const exception &th) {
  }
}


void TIDorb::core::poa::IIOPLocateRequest::dispatch(TIDorb::core::poa::ExecThread * exec,
                                                    TIDorb::core::poa::POAImpl * poa)
{
  exec->execute(this,poa);
}



// void* TIDorb::core::poa::IIOPLocateRequest::_impl()
// {
// 	return this;
// }

// const char* TIDorb::core::poa::IIOPLocateRequest::_typeid()
// {
// 	//return CORBA::string_dup("IIOPLocateRequest");
// 	return "IIOPLocateRequest";
// }

