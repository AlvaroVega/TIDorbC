/*/////////////////////////////////////////////////////////////////////////
//
// File:        Stub.h
// Description: 
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////*/

#include "CORBA.h"

#ifndef _TIDORB_PORTABLE_OBJECT_H_
#define _TIDORB_PORTABLE_OBJECT_H_


namespace TIDorb {
namespace portable {

class Stub : public virtual CORBA::Object,
             public virtual TIDorb::portable::RefCounter
{
  public:

    virtual ~Stub();
#ifndef MINIMUN
    CORBA::InterfaceDef_ptr _get_interface();
#endif
    CORBA::Boolean _is_a(const char* logical_type_id);

    CORBA::Boolean _non_existent();

    CORBA::Boolean _is_equivalent(CORBA::Object_ptr other_object);

    CORBA::ULong _hash(CORBA::ULong maximum);

    void _create_request(CORBA::Context_ptr ctx,
                         const char *operation,
                         CORBA::NVList_ptr arg_list,
                         CORBA::NamedValue_ptr result,
                         CORBA::Request_out request,
                         CORBA::Flags req_flags);

    void _create_request(CORBA::Context_ptr ctx,
                         const char *operation,
                         CORBA::NVList_ptr arg_list,
                         CORBA::NamedValue_ptr result,
                         CORBA::ExceptionList_ptr excp_list,
                         CORBA::ContextList_ptr ctx_list,
                         CORBA::Request_out request,
                         CORBA::Flags req_flags);

    CORBA::Request_ptr _request(const char* operation);

    CORBA::Policy_ptr _get_policy(CORBA::PolicyType policy_type);

    CORBA::DomainManagerList* _get_domain_managers();

    CORBA::Object_ptr _set_policy_overrides(const CORBA::PolicyList& policies,
                                            CORBA::SetOverrideType set_or_add);

    CORBA::Policy_ptr _get_client_policy(CORBA::PolicyType policy_type);

    CORBA::PolicyList_ptr _get_policy_overrides(const CORBA::PolicyTypeSeq& types);

    CORBA::Boolean _validate_connection(CORBA::PolicyList_out inconsistent_policies);

    //TIDorb

    // Repository ids
    virtual const CORBA::RepositoryIdSeq_ptr _ids() = 0;

    // ORB
    TIDorb::portable::ORB* _orb();

    // The reference has been created by the orb()

    ::CORBA::Boolean _is_local();

    // Reference delegate
    void _set_delegate(TIDorb::portable::ObjectDelegate* d);
    TIDorb::portable::ObjectDelegate* _get_delegate();

    TIDorb::portable::ObjectDelegate* m_delegate;

  protected:
    Stub();
};

}
}

#endif
