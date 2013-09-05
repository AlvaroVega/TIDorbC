#include "CORBA.h"


#ifndef _CORBA_SERVER_REQUEST_H_
#define _CORBA_SERVER_REQUEST_H_


class ServerRequest
{
  public:
    virtual ~ServerRequest() {}
    
    virtual const char* operation() const = 0;
    virtual void arguments(CORBA::NVList_ptr& parameters)= 0;
    virtual CORBA::Context_ptr ctx()= 0;
    virtual void set_result(const CORBA::Any& value)= 0;
    virtual void set_exception(const CORBA::Any& value)= 0;

  protected:
    ServerRequest(){}
};

#endif
