#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_UNKNOWN_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_UNKNOWN_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class UNKNOWN_Creator : public virtual SystemExceptionCreator {
    
  public:

   UNKNOWN_Creator(){}
   virtual ~UNKNOWN_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::UNKNOWN(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::UNKNOWN(dynamic_cast<const CORBA::UNKNOWN&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
