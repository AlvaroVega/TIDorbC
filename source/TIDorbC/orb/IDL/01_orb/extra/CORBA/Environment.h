#include "CORBA.h"


#ifndef _CORBA_ENVIRONMENT_H_
#define _CORBA_ENVIRONMENT_H_

  
class Environment
{
  public:
  
    virtual ~Environment(){}
    
    virtual void exception(Exception* e) = 0;

    virtual Exception *exception() const = 0;
    
    virtual void clear() = 0;

    static Environment_ptr _duplicate(Environment_ptr ev);
    static Environment_ptr _nil();
    
  protected:
    Environment() {}
};


#endif //_CORBA_ENVIRONMENT_H_
