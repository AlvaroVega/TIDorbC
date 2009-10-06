/////////////////////////////////////////////////////////////////////////
//
// File:        RebindPolicyImpl.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2005
//
// Revised:
//
// Copyright 2005 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/messaging.h"

#ifndef _TIDORB_CORE_REBIND_POLICY_IMPL_H_
#define _TIDORB_CORE_REBIND_POLICY_IMPL_H_

#include "CORBA.h"
#include "Messaging.h"


namespace TIDorb {
namespace core {
namespace messaging {

class RebindPolicyImpl: public virtual Messaging::RebindPolicy,
                        public virtual CORBA::LocalObject,
                        virtual public TIDThr::RefCounter {

private:
  
  Messaging::RebindMode m_value;

  bool destroyed;

public:

    RebindPolicyImpl(Messaging::RebindMode val);

    ~RebindPolicyImpl() throw (TIDThr::SystemException);

    Messaging::RebindMode rebind_mode();

    CORBA::PolicyType policy_type();

    CORBA::Policy_ptr copy() throw(CORBA::SystemException);
    
    void destroy();
    
};
}// messaging
}// core
}// TIDorb
#endif
