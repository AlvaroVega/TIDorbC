#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_TRANSACTION_REQUIRED_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_TRANSACTION_REQUIRED_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class TRANSACTION_REQUIRED_Creator : public virtual SystemExceptionCreator {
    
  public:

   TRANSACTION_REQUIRED_Creator(){}
   virtual ~TRANSACTION_REQUIRED_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::TRANSACTION_REQUIRED(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::TRANSACTION_REQUIRED(dynamic_cast<const CORBA::TRANSACTION_REQUIRED&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
