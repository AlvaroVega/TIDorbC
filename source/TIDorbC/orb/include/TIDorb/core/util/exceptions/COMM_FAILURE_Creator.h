#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_COMM_FAILURE_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_COMM_FAILURE_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class COMM_FAILURE_Creator : public virtual SystemExceptionCreator {
    
  public:

   COMM_FAILURE_Creator(){}
   virtual ~COMM_FAILURE_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::COMM_FAILURE(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::COMM_FAILURE(dynamic_cast<const CORBA::COMM_FAILURE&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
