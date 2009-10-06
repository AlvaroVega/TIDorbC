//////////////////////////////////////////////////////////////////////////////////
//
// File:        PolicyCurrentImpl.h
// Description: PolicyCurrent implementation.
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

#ifndef _TIDORB_CORE_POLICY_CURRENT_IMPL_H
#define _TIDORB_CORE_POLICY_CURRENT_IMPL_H


namespace TIDorb {
namespace core {


class PolicyCurrentImpl: public virtual CORBA::PolicyCurrent,
                         public virtual CORBA::LocalObject
{

  TIDorb::core::PolicyContextManagerImpl* m_manager;

public:
  PolicyCurrentImpl(TIDorb::core::PolicyContextManagerImpl* manager);

  ~PolicyCurrentImpl();

  CORBA::PolicyList* get_policy_overrides(const CORBA::PolicyTypeSeq& ts);

  void set_policy_overrides(const CORBA::PolicyList& policies,
                            CORBA::SetOverrideType set_add)
    throw (CORBA::InvalidPolicies);
  
  TIDorb::core::PolicyContext* getPolicyContext();
  
};


} //core
} //TIDorb

#endif
