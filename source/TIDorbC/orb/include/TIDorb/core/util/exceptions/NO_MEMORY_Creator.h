#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_NO_MEMORY_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_NO_MEMORY_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class NO_MEMORY_Creator : public virtual SystemExceptionCreator {
    
  public:

   NO_MEMORY_Creator(){}
   virtual ~NO_MEMORY_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::NO_MEMORY(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::NO_MEMORY(dynamic_cast<const CORBA::NO_MEMORY&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
