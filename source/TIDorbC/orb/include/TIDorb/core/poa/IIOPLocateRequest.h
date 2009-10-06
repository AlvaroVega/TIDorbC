/////////////////////////////////////////////////////////////////////////
//
// File:        IIOPLocateRequest.h
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


#ifndef _TIDORB_CORE_POA_IIOPLocalRequest_H_
#define _TIDORB_CORE_POA_IIOPLocalRequest_H_


namespace TIDorb {
namespace core {
namespace poa {


class IIOPLocateRequest : public TIDorb::core::poa::AbstractIIOPRequest
{
  private:
    TIDorb::core::comm::iiop::RequestId _id;
    TIDorb::core::comm::iiop::Version _version;
    const char* iiop_locate_name;

  public:
    IIOPLocateRequest(TIDorb::core::poa::POAKey* poakey, 
                      TIDorb::core::poa::POAImpl* root_poa,
                      TIDorb::core::comm::Connection* conn, 
                      TIDorb::core::comm::iiop::RequestId id,
                      const TIDorb::core::comm::iiop::Version& ver);

    ~IIOPLocateRequest();

    TIDorb::core::comm::iiop::RequestId get_id() const;

    const TIDorb::core::comm::iiop::Version& get_version() const;

    const char* toString() const;

    void returnError(const CORBA::SystemException& e,
                     TIDorb::util::Trace* trace);

    void dispatch(ExecThread * exec, POAImpl* poa);

//     virtual void* _impl();
//     virtual const char* _typeid();
};


} //poa
} //core
} //TIDorb


#endif
