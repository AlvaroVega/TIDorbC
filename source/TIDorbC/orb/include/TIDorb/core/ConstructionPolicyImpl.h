//////////////////////////////////////////////////////////////////////////////////
//
// File:        ConstructionPolicyImpl.h
// Description: ConstructionPolicy implementation.
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

#ifndef __TIDorb_core_ConstructionPolicyImpl_H_
#define __TIDorb_core_ConstructionPolicyImpl_H_

#include "CORBA.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {

class ConstructionPolicyImpl: public virtual CORBA::ConstructionPolicy,
                              public virtual CORBA::LocalObject,
                              virtual public TIDThr::RefCounter {

private:
  typedef map<string, CORBA::Boolean>  ConstrMapTable;
  ConstrMapTable _constr;

  bool destroyed;
  
public:
  ConstructionPolicyImpl();

  virtual ~ConstructionPolicyImpl() throw (TIDThr::SystemException);

  

  CORBA::PolicyType policy_type();
 
  void make_domain_manager (CORBA::InterfaceDef_ptr object_type,
                            CORBA::Boolean constr_policy);

  CORBA::Boolean constr_policy (CORBA::Object_ptr object);

  CORBA::Policy_ptr copy() throw(CORBA::SystemException);
  
  void destroy();

  
};

}
}

#endif

