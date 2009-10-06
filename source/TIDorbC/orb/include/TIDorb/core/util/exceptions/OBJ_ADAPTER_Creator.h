#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_OBJ_ADAPTER_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_OBJ_ADAPTER_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class OBJ_ADAPTER_Creator : public virtual SystemExceptionCreator {
    
  public:

   OBJ_ADAPTER_Creator(){}
   virtual ~OBJ_ADAPTER_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::OBJ_ADAPTER(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::OBJ_ADAPTER(dynamic_cast<const CORBA::OBJ_ADAPTER&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
