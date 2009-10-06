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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

  protected:
//MLG  
    //TIDORB_ref m_orb;
    TIDORB* m_orb;
//EMLG    
    TIDorb::core::iop::IOR_ref m_reference;
    TIDorb::templates::RefCount_var<ObjectDelegateImpl> m_forwarded_delegate;    
    TIDorb::core::comm::CommunicationLayer_ref m_comm_layer;   

    TIDorb::core::PolicyContext* policyContext;     

    ObjectDelegateImpl(TIDORB* orb, 
                       TIDorb::core::iop::IOR* ref,
                       TIDorb::core::comm::CommunicationLayer* comm_layer,
                       TIDorb::core::PolicyContext* policies)
    throw (TIDThr::SystemException);

};

}
}

#endif
