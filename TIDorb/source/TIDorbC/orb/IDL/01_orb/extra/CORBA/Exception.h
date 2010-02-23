#include "CORBA.h"


#ifndef _CORBA_EXCEPTION_H_
#define _CORBA_EXCEPTION_H_

  
class Exception
{
  public:
    Exception(const Exception &){}
    
    virtual ~Exception(){}
    Exception& operator=(const Exception &){return *this;}
    
    virtual void _raise() const {}
    virtual const char* _name() const { return ""; }
    virtual const char* _rep_id() const { return ""; }

  protected:
    Exception(){}
};





#endif
