#include "CORBA.h"


#ifndef _CORBA_REQUEST_H_
#define _CORBA_REQUEST_H_


const Flags OUT_LIST_MEMORY = 1;
const Flags IN_COPY_VALUE   = 2;

  
class Request
{
  public:
  
    virtual ~Request() {}
    
    virtual Object_ptr target() const = 0;
    virtual const char *operation() const = 0;
    virtual NVList_ptr arguments() = 0;
    virtual NamedValue_ptr result() = 0;
    virtual Environment_ptr env() = 0;
    virtual ExceptionList_ptr exceptions() = 0;
    virtual ContextList_ptr contexts() = 0;
    virtual void ctx(Context_ptr) = 0;
    virtual Context_ptr ctx() const = 0;
    virtual Any& add_in_arg() = 0;
    virtual Any& add_in_arg(const char* name) = 0;
    virtual Any& add_inout_arg() = 0;
    virtual Any& add_inout_arg(const char* name) = 0;
    virtual Any& add_out_arg() = 0;
    virtual Any& add_out_arg(const char* name) = 0;
    virtual void set_return_type(TypeCode_ptr tc) = 0;
    virtual Any& return_value() = 0;
    virtual void invoke() = 0;
    virtual void send_oneway() = 0;
    virtual void send_deferred() = 0;
    virtual void get_response() = 0;
    virtual Boolean poll_response() = 0;
    
    static Request_ptr _duplicate(Request_ptr req);
    static Request_ptr _nil();
    
  protected:
    Request() {}
//MLG
  public:
    virtual void* _impl() = 0;
    virtual const char* _typeid() = 0;
//EMLG    
};

#endif
