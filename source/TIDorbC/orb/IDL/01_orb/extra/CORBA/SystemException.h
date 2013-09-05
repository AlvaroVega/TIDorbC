#include "CORBA.h"

#include "CORBA.h"

#ifndef _CORBA_SYSTEM_EXCEPTION_H_
#define _CORBA_SYSTEM_EXCEPTION_H_

#ifdef TIDORB_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif


#ifdef minor
#define __CORBA_SYSTEM_EXCEPTION_MINOR_ minor
#undef minor
#endif


class SystemException : public virtual Exception
{
  public:  
    
    virtual ~SystemException()
    {
      CORBA::string_free(m_reason);
    }
    
    ULong minor() const 
    {
      return m_minor;
    }
    
    void minor(ULong value)
    {
      m_minor = value;
    }
       
    CompletionStatus completed() const 
    {
      return m_completed;
    }
    
    void completed(CompletionStatus value)
    {
      m_completed = value;
    }

    // TIDorb operation
    
    const CORBA::TypeCode_ptr type()
    {
        return m_type;
    }
    // TIDorb operation
    
    const char* what() const throw ()
    {
      return m_reason;
    }
    
  //    virtual void _raise() const; // = 0;
    
    static SystemException* _downcast(Exception* e)
    {
      return dynamic_cast<SystemException*>(e);
    }
    
    static const SystemException* _downcast(const Exception* e)
    {
      return dynamic_cast<SystemException*>(const_cast<Exception*>(e));
    }
      

    SystemException(const SystemException &e) 
      : Exception(e), 
       m_minor(e.m_minor), 
       m_completed(e.m_completed), 
       m_type(e.m_type) 
    {
      m_reason = CORBA::string_dup(e.m_reason);
    }

    SystemException(const char* reason, 
                    ULong minor, 
                    CompletionStatus status,
                    const CORBA::TypeCode_ptr type)
      : m_minor(minor), m_completed(status), m_type(type)
    {
      m_reason = CORBA::string_dup(reason);
    }
    
    SystemException& operator=(const SystemException &e)
    {
      CORBA::string_free(m_reason);
      m_reason = CORBA::string_dup(e.m_reason);
      m_minor = e.m_minor;
      m_completed = e.m_completed;
      m_type = e.m_type;
      return *this;      
    }  
    

  protected:

    SystemException() 
      : m_reason(0), m_minor(0), m_completed(COMPLETED_NO), m_type(0) 
    {}
    
  protected:
    char * m_reason;
    ULong m_minor; 
    CompletionStatus m_completed;
    CORBA::TypeCode_ptr m_type;
};

#ifdef __CORBA_SYSTEM_EXCEPTION_MINOR__
#define minor __CORBA_SYSTEM_EXCEPTION_MINOR__
#undef __CORBA_SYSTEM_EXCEPTION_MINOR__
#endif




#endif
