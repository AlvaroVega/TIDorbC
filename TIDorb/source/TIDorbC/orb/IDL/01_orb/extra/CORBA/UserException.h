#include "CORBA.h"


#ifndef _CORBA_UserException_H_
#define _CORBA_UserException_H_


class UserException : public virtual Exception
{
    public:
      virtual ~UserException(){}
      //virtual void _raise() const; // = 0;


    static UserException* _downcast(Exception* e)
    {
      return dynamic_cast<UserException*>(e);
    }
    
    static const UserException* _downcast(const Exception* e)
    {
      return dynamic_cast<UserException*>(const_cast<Exception*>(e));
    }

    UserException(const UserException &e) : Exception(e) {}
    //UserException &operator=(const UserException &){ return *this;}
    
  protected:
    UserException() {}   
};


#endif
