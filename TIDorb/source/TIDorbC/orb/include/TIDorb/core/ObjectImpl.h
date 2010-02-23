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
