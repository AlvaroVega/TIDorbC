#include "CORBA.h"


#ifndef _CORBA_UNKNOWN_USER_EXCEPTION_H_
#define _CORBA_UNKNOWN_USER_EXCEPTION_H_


class UnknownUserException : public virtual UserException
{
  public:
    
    UnknownUserException(const UnknownUserException& u)
      : Exception(u),
        UserException(u) ,
        m_exception(u.m_exception) {}
        
    UnknownUserException(const Any& e) : m_exception(e) {}
    
    ~UnknownUserException() {}
        
    Any& exception()
    {
      return m_exception;
    }
    
    void _raise() const
    {
      throw UnknownUserException(*this);
    }

    const char* _name() const
    {
      return "UnknownUserException";
    }
    
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/UnknownUserException:1.0";
    }
  
    static UnknownUserException* _downcast(Exception* e)
    {
      return dynamic_cast<UnknownUserException*>(e);
    }
    
    static const UnknownUserException* _downcast(const Exception* e)
    {
      return dynamic_cast<UnknownUserException*>(const_cast<Exception*>(e));
    }
    
  private:
    Any m_exception;
};


#endif
