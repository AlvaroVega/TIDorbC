#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_CODESET_INCOMPATIBLE_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_CODESET_INCOMPATIBLE_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class CODESET_INCOMPATIBLE_Creator : public virtual SystemExceptionCreator {
    
  public:

   CODESET_INCOMPATIBLE_Creator(){}
   virtual ~CODESET_INCOMPATIBLE_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::CODESET_INCOMPATIBLE(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::CODESET_INCOMPATIBLE(dynamic_cast<const CORBA::CODESET_INCOMPATIBLE&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
