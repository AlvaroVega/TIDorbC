#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_TRANSIENT_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_TRANSIENT_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util{
namespace exceptions {


class TRANSIENT_Creator : public virtual SystemExceptionCreator {
    
  public:

   TRANSIENT_Creator(){}
   virtual ~TRANSIENT_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::TRANSIENT(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::TRANSIENT(dynamic_cast<const CORBA::TRANSIENT&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
