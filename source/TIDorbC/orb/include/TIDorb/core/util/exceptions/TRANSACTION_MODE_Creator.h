#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_TRANSACTION_MODE_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_TRANSACTION_MODE_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class TRANSACTION_MODE_Creator : public virtual SystemExceptionCreator {
    
  public:

   TRANSACTION_MODE_Creator(){}
   virtual ~TRANSACTION_MODE_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::TRANSACTION_MODE(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::TRANSACTION_MODE(dynamic_cast<const CORBA::TRANSACTION_MODE&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
