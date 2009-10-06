#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_REBIND_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_REBIND_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class REBIND_Creator : public virtual SystemExceptionCreator {
    
  public:

   REBIND_Creator(){}
   virtual ~REBIND_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::REBIND(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::REBIND(dynamic_cast<const CORBA::REBIND&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
