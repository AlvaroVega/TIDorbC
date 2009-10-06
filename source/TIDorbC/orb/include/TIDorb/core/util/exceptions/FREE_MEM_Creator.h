#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_FREE_MEM_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_FREE_MEM_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class FREE_MEM_Creator : public virtual SystemExceptionCreator {
    
  public:

   FREE_MEM_Creator(){}
   virtual ~FREE_MEM_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::FREE_MEM(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::FREE_MEM(dynamic_cast<const CORBA::FREE_MEM&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};


} // exceptions
} // util
} // core
} // TIDorb

#endif 
