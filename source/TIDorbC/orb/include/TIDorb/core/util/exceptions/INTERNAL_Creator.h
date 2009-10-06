#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_INTERNAL_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_INTERNAL_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {


class INTERNAL_Creator : public virtual SystemExceptionCreator {
    
  public:

   INTERNAL_Creator(){}
   virtual ~INTERNAL_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::INTERNAL(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::INTERNAL(dynamic_cast<const CORBA::INTERNAL&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
