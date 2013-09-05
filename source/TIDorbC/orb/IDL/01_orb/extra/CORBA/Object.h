#include "CORBA.h"


#ifndef _CORBA_OBJECT_H_
#define _CORBA_OBJECT_H_

  
class Object
{
public:
  
  virtual ~Object() {}    
#ifndef MINIMUN
  virtual InterfaceDef_ptr _get_interface() = 0;
#endif
  virtual Boolean _is_a(const char* logical_type_id) = 0;
    
  virtual Boolean _non_existent() = 0;
    
  virtual Boolean _is_equivalent(Object_ptr other_object) = 0;
    
  virtual ULong _hash(ULong maximum) = 0;

    
  virtual void _create_request(Context_ptr ctx, 
                               const char *operation,
                               NVList_ptr arg_list, 
                               NamedValue_ptr result,
                               Request_out request, 
                               Flags req_flags) = 0;
                         
  virtual void _create_request(Context_ptr ctx,
                               const char *operation,
                               NVList_ptr arg_list, 
                               NamedValue_ptr result,
                               ExceptionList_ptr excp_list,
                               ContextList_ptr ctx_list,
                               Request_out request,
                               Flags req_flags) = 0;

                         
  virtual Request_ptr _request(const char* operation) = 0;


  virtual Policy_ptr _get_policy(PolicyType policy_type) = 0;

 
    
  virtual DomainManagerList* _get_domain_managers() = 0;
  
  virtual Object_ptr _set_policy_overrides(const PolicyList& policies,
                                           SetOverrideType set_or_add) = 0;

  virtual Policy_ptr _get_client_policy(PolicyType policy_type) = 0;
  virtual PolicyList_ptr _get_policy_overrides(const PolicyTypeSeq& types) = 0;
  virtual Boolean _validate_connection(PolicyList_out inconsistent_policies ) = 0;

  static Object_ptr _duplicate(Object_ptr obj);
  static Object_ptr _nil();
  
  protected:
    Object(){}
};

#endif
