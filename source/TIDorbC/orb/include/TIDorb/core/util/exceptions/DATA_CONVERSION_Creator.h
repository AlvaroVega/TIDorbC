#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_DATA_CONVERSION_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_DATA_CONVERSION_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class DATA_CONVERSION_Creator : public virtual SystemExceptionCreator {
    
  public:

   DATA_CONVERSION_Creator(){}
   virtual ~DATA_CONVERSION_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::DATA_CONVERSION(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::DATA_CONVERSION(dynamic_cast<const CORBA::DATA_CONVERSION&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
