#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_TRANSACTION_ROLLEDBACK_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_TRANSACTION_ROLLEDBACK_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class TRANSACTION_ROLLEDBACK_Creator : public virtual SystemExceptionCreator {
    
  public:

   TRANSACTION_ROLLEDBACK_Creator(){}
   virtual ~TRANSACTION_ROLLEDBACK_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::TRANSACTION_ROLLEDBACK(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::TRANSACTION_ROLLEDBACK(dynamic_cast<const CORBA::TRANSACTION_ROLLEDBACK&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
