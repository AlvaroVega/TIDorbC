#include "CORBA.h"


#ifndef _CORBA_EXCEPTION_LIST_H_
#define _CORBA_EXCEPTION_LIST_H_

  
class ExceptionList
{
  public:
    virtual ~ExceptionList() {}
    
    virtual ULong count() const = 0;
    virtual void add(TypeCode_ptr tc) = 0;
    virtual void add_consume(TypeCode_ptr tc) = 0;
    virtual TypeCode_ptr item(ULong index) = 0;
    virtual void remove(ULong index) = 0;
    
    static ExceptionList_ptr _duplicate(ExceptionList_ptr el);
    static ExceptionList_ptr _nil();
    
  protected:
    ExceptionList(){}
};


#endif //_CORBA_EXCEPTION_LIST_H_
