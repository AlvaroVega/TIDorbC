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
