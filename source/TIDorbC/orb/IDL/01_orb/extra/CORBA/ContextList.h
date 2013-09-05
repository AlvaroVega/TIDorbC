#include "CORBA.h"


#ifndef _CORBA_ContextList_H_
#define _CORBA_ContextList_H_


class ContextList
{
  public:
    virtual ~ContextList(){}
  
    virtual ULong count() const = 0;
    virtual void add(const char* ctxt) = 0;
    virtual void add_consume(char* ctxt) = 0;
    virtual const char* item(ULong index) = 0;
    virtual void remove(ULong index) = 0;
    
    static ContextList_ptr _duplicate(ContextList_ptr cl);
    static ContextList_ptr _nil();
    
  protected:
    ContextList(){}
};


#endif
