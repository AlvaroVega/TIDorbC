/////////////////////////////////////////////////////////////////////////
//
// File:        QueuedRequest.h
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

#ifndef _TIDORB_CORE_POA_QueuedRequest_H_
#define _TIDORB_CORE_POA_QueuedRequest_H_

#include "TIDorb/util/Trace.h"

namespace TIDorb {
namespace core {
namespace poa {

class ExecThread;
class POAImpl;

class QueuedRequest
{
  private:
    TIDorb::core::poa::POAKey_ref _poakey;

    CORBA::ULong _current_child_poa_name_level;

    bool mustDiscard;

    CORBA::CompletionStatus status;

    TIDorb::core::poa::POAImpl* _current_poa;
    
    CORBA::Long m_serial;

    TIDorb::core::PolicyContext* m_policy_context;

  public:
    QueuedRequest(TIDorb::core::poa::POAKey*  poakey = 0,
                  TIDorb::core::poa::POAImpl* root_poa = 0);

    virtual ~QueuedRequest();

    CORBA::CompletionStatus getStatus() const;

    void setStatus(CORBA::CompletionStatus value);

    const char* get_current_child_POA_name() const;

    TIDorb::core::poa::POAImpl* get_current_POA() const;

    void next_child_POA(TIDorb::core::poa::POAImpl* poa);

    TIDorb::core::poa::POAKey* getPOAKey() const;

    const TIDorb::core::poa::OID& getOID() const;

    bool isFinalPOA() const;

    void setMustDiscard(bool value);

    bool getMustDiscard() const;

    void setSerial(CORBA::Long serial); 

    CORBA::Long getSerial() const;

    void setPolicyContext(TIDorb::core::PolicyContext* context);

    TIDorb::core::PolicyContext* getPolicyContext() const;

    // Requests MUST be eliminated by calling destroy() method. This simply
    // deletes the object, except in LocalRequests where reference counting is
    // necessary to avoid race conditions in deleting them (both POAManager's
    // ExecThread and the invoking thread try to eliminate the request)
    /*void destroy();*/
    virtual void destroy();

    virtual void returnError(const CORBA::SystemException& e,
                             TIDorb::util::Trace* trace) = 0;

    virtual const char* toString() const = 0;
  
    // Implements double dispathing
    virtual void dispatch (ExecThread * exec, POAImpl * poa) = 0;
   
//     virtual void* _impl();
//     virtual const char* _typeid();
};

} //poa
} //core
} //TIDorb


#endif
