#include "CORBA.h"


#ifndef _CORBA_LOCAL_OBJECT_H_
#define _CORBA_LOCAL_OBJECT_H_


class LocalObject : public virtual Object
{
  public:
    virtual ~LocalObject();

    virtual void _add_ref();
    virtual void _remove_ref();
#ifndef MINIMUN
    InterfaceDef_ptr _get_interface();
#endif
    Boolean _is_a(const char* logical_type_id);

    Boolean _non_existent();

    Boolean _is_equivalent(Object_ptr other_object);

    ULong _hash(ULong maximum);

    void _create_request(Context_ptr ctx,
                         const char *operation,
                         NVList_ptr arg_list,
                         NamedValue_ptr result,
                         Request_out request,
                         Flags req_flags);

    void _create_request(Context_ptr ctx,
                         const char *operation,
                         NVList_ptr arg_list,
                         NamedValue_ptr result,
                         ExceptionList_ptr excp_list,
                         ContextList_ptr ctx_list,
                         Request_out request,
                         Flags req_flags);

    Request_ptr _request(const char* operation);

    Policy_ptr _get_policy(PolicyType policy_type);

    DomainManagerList* _get_domain_managers();

    Object_ptr _set_policy_overrides(const PolicyList& policies,
                                     SetOverrideType set_or_add);

    Policy_ptr _get_client_policy(PolicyType policy_type);
    PolicyList_ptr _get_policy_overrides(const PolicyTypeSeq& types);
    Boolean _validate_connection(PolicyList_out inconsistent_policies );

    static LocalObject_ptr _narrow(Object_ptr obj);
    static LocalObject_ptr _duplicate(LocalObject_ptr obj);
    static LocalObject_ptr _nil();

  protected:
    LocalObject();
//MLG
  public:
    virtual void* _impl();
    virtual const char* _typeid();
//EMLG
};

#endif
