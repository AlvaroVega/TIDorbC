/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        ObjectImpl.h
// Description: CORBA::Object implementation.
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core.h"

#ifndef _TIDorb_core_ObjectImpl_H_
#define _TIDorb_core_ObjectImpl_H_

#include "CORBA.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {

class ObjectImpl : public virtual TIDorb::portable::Stub
{
  public:
    ObjectImpl() {}
    ~ObjectImpl() {}
    const CORBA::RepositoryIdSeq_ptr _ids();
    static TIDorb::core::ObjectImpl* fromIOR(TIDorb::core::TIDORB* orb, TIDorb::core::iop::IOR* ior);
    CORBA::Policy_ptr _get_policy(CORBA::PolicyType policy_type);
    CORBA::Object_ptr _set_policy_overrides(const CORBA::PolicyList& policies,
                                            CORBA::SetOverrideType set_or_add);
    CORBA::PolicyList_ptr _get_policy_overrides(const CORBA::PolicyTypeSeq& types);

  protected:
    static CORBA::RepositoryIdSeq_ptr __init_ids();
    static const CORBA::RepositoryIdSeq_ptr __ids;
};

}
}

#endif
