/////////////////////////////////////////////////////////////////////////
//
// File:        LocalRequest.h
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

#include "TIDorb/core/poa.h"

#ifndef _TIDORB_CORE_POA_LocalRequest_H_
#define _TIDORB_CORE_POA_LocalRequest_H_


namespace TIDorb {
namespace core {
namespace poa {


class LocalRequest;
typedef TIDThr::HandleT<LocalRequest> LocalRequest_ref; /* reference counting */


class LocalRequest : public TIDorb::core::poa::AbstractLocalRequest
{
private:

  TIDorb::core::util::OperationCompletion state;

  TIDorb::core::LocalServerRequest* request;

  char* local_name;

public:
  LocalRequest(TIDorb::core::poa::POAKey* poakey, 
               TIDorb::core::poa::POAImpl* root_poa, 
               TIDorb::core::LocalServerRequest* req);

  ~LocalRequest() throw(TIDThr::SystemException); 

  TIDorb::core::LocalServerRequest* get_request();

  bool is_completed();

  void set_completed();

  void wait_for_completion(CORBA::ULong how_long) ;

  const char* toString() const;

  void returnError(const CORBA::SystemException& e, TIDorb::util::Trace* trace);

  void dispatch (ExecThread * exec,POAImpl  * poa);

//   void* _impl();
//   const char* _typeid();
};


} //poa
} //core
} //TIDorb


#endif


