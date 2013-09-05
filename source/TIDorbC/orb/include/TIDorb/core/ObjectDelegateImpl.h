//////////////////////////////////////////////////////////////////////////////////
//
// File:        ObjectDelegateImpl.h
// Description: ObjectDelegate implementation.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_OBJECT_DELEGATE_IMPL_H_
#define _TIDORB_CORE_OBJECT_DELEGATE_IMPL_H_

#include "CORBA.h"
#include "TIDThr.h"


namespace TIDorb {
namespace core {

class ObjectDelegateImpl : public TIDorb::portable::ObjectDelegate
{
  public:
    ObjectDelegateImpl(TIDORB* orb, 
                       TIDorb::core::iop::IOR* ref,
                       TIDorb::core::comm::CommunicationLayer* comm_layer)
    throw (TIDThr::SystemException);
    
    ~ObjectDelegateImpl() throw (TIDThr::SystemException);

    // ORB
    TIDorb::portable::ORB* orb();

    // The reference has been created by the orb()

    ::CORBA::Boolean is_local();
    
    ::CORBA::Boolean non_existent();

    TIDorb::core::iop::IOR* getReference();
    
    void invoke(RequestImpl* request);

    void oneway_request(RequestImpl* request) throw(CORBA::SystemException);
 
    TIDorb::core::PolicyContext* getPolicyContext();

    TIDorb::core::PolicyContext* createRequestPolicyContext();

    ObjectDelegateImpl* copy();

    CORBA::DomainManagerList* getDomainManagerList();

  protected:

    TIDORB* m_orb;

    TIDorb::core::iop::IOR_ref m_reference;
    TIDorb::templates::RefCount_var<ObjectDelegateImpl> m_forwarded_delegate;    
    TIDorb::core::comm::CommunicationLayer_ref m_comm_layer;   

    TIDorb::core::PolicyContext* policyContext;     

    ObjectDelegateImpl(TIDORB* orb, 
                       TIDorb::core::iop::IOR* ref,
                       TIDorb::core::comm::CommunicationLayer* comm_layer,
                       TIDorb::core::PolicyContext* policies)
    throw (TIDThr::SystemException);

    CORBA::DomainManagerList* m_domain_manager_list;     

};

}
}

#endif
