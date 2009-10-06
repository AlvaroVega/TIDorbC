#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_INVALID_TRANSACTION_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_INVALID_TRANSACTION_CREATOR_H_


#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class INVALID_TRANSACTION_Creator : public virtual SystemExceptionCreator {
    
  public:

   INVALID_TRANSACTION_Creator(){}
   virtual ~INVALID_TRANSACTION_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::INVALID_TRANSACTION(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::INVALID_TRANSACTION(dynamic_cast<const CORBA::INVALID_TRANSACTION&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
