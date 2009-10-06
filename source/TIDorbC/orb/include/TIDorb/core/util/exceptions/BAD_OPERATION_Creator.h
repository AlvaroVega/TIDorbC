#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_BAD_OPERATION_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_BAD_OPERATION_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class BAD_OPERATION_Creator : public virtual SystemExceptionCreator {
    
  public:

   BAD_OPERATION_Creator(){}
   virtual ~BAD_OPERATION_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::BAD_OPERATION(minor,completion);
  }

  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::BAD_OPERATION(dynamic_cast<const CORBA::BAD_OPERATION&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
