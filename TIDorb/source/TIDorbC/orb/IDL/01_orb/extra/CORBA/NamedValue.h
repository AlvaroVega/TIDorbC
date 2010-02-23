#include "CORBA.h"


#ifndef _CORBA_NAMED_VALUE_H_
#define _CORBA_NAMED_VALUE_H_

  
class NamedValue
{
  public:
    virtual ~NamedValue(){}
    
    virtual const char *name() const = 0;
    virtual Any *value() const = 0;
    
    virtual Flags flags() const = 0;
    
    static NamedValue_ptr _duplicate(NamedValue_ptr nv);
    static NamedValue_ptr _nil();
    
  protected:
    NamedValue(){}
//MLG
  public:
    virtual void* _impl() = 0;
    virtual const char* _typeid() = 0;
//EMLG
};

#endif //_CORBA_NAMED_VALUE_H_
