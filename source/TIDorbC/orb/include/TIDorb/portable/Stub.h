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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

    CORBA::InterfaceDef_ptr _get_interface();

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
