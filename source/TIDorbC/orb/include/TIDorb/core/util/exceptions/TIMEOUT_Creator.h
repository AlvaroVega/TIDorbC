#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_TIMEOUT_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_TIMEOUT_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util{
namespace exceptions {


class TIMEOUT_Creator : public virtual SystemExceptionCreator {
    
  public:

   TIMEOUT_Creator(){}
   virtual ~TIMEOUT_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::TIMEOUT(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::TIMEOUT(dynamic_cast<const CORBA::TIMEOUT&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
