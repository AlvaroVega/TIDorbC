#include "CORBA.h"

#ifndef _CORBA_AbstractBase_H_
#define _CORBA_AbstractBase_H_



class AbstractBase
{
  public:
  
    static AbstractBase_ptr _duplicate(AbstractBase_ptr ab);
    static AbstractBase_ptr _narrow(AbstractBase_ptr ab);
    static AbstractBase_ptr _nil();
    
    Object_ptr _to_object();
    ValueBase* _to_value();
  
  protected:
    AbstractBase();
    AbstractBase(const AbstractBase& val);
    virtual ~AbstractBase() {}
};

#endif
