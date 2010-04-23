//////////////////////////////////////////////////////////////////////////////////
//
// File:        DomainManagerImpl.h
// Description: DomainManager implementation.
//
// Rel:         01.00
// Created:     September, 2009
// Author:      
//
// Revised:
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#ifndef __TIDorb_core_DomainManagerImpl_H_
#define __TIDorb_core_DomainManagerImpl_H_

#include "CORBA.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {

class DomainManagerImpl: public virtual CORBA::DomainManager,
                         public virtual CORBA::LocalObject,
                         virtual public TIDThr::RefCounter
{
protected:

  TIDorb::core::TIDORB* _orb;

  typedef map<CORBA::PolicyType, CORBA::Policy_var> PolicyTableT;
  
  PolicyTableT m_policy_table;
  
public:

  DomainManagerImpl(TIDorb::core::TIDORB* orb);

  virtual ~DomainManagerImpl() throw (TIDThr::SystemException);
    
  CORBA::Policy_ptr get_domain_policy(CORBA::PolicyType policy_type);
  
  void set_domain_policy (CORBA::Policy_ptr policy);

  // CORBA::DomainManager_ptr copy ();
  
};

}
}

#endif

