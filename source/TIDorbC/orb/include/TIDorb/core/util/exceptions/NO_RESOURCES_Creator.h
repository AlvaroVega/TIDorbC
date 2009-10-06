#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_NO_RESOURCES_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_NO_RESOURCES_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class NO_RESOURCES_Creator : public virtual SystemExceptionCreator {
    
  public:

   NO_RESOURCES_Creator(){}
   virtual ~NO_RESOURCES_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::NO_RESOURCES(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::NO_RESOURCES(dynamic_cast<const CORBA::NO_RESOURCES&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
