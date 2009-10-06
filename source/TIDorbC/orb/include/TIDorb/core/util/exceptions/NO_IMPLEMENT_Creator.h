#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_NO_IMPLEMENT_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_NO_IMPLEMENT_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class NO_IMPLEMENT_Creator : public virtual SystemExceptionCreator {
    
  public:

   NO_IMPLEMENT_Creator(){}
   virtual ~NO_IMPLEMENT_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::NO_IMPLEMENT(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::NO_IMPLEMENT(dynamic_cast<const CORBA::NO_IMPLEMENT&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
