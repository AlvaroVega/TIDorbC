#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_EXCEPTIONS_IMP_LIMIT_CREATOR_H_
#define _TIDORB_CORE_EXCEPTIONS_IMP_LIMIT_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class IMP_LIMIT_Creator : public virtual SystemExceptionCreator {
    
  public:

   IMP_LIMIT_Creator(){}
   virtual ~IMP_LIMIT_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::IMP_LIMIT(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::IMP_LIMIT(dynamic_cast<const CORBA::IMP_LIMIT&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
