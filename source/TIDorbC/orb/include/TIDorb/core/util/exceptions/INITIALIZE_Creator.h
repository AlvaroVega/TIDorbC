#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_INITIALIZE_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_INITIALIZE_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class INITIALIZE_Creator : public virtual SystemExceptionCreator {
    
  public:

   INITIALIZE_Creator(){}
   virtual ~INITIALIZE_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::INITIALIZE(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::INITIALIZE(dynamic_cast<const CORBA::INITIALIZE&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
