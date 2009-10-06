#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_INV_OBJREF_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_INV_OBJREF_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class INV_OBJREF_Creator : public virtual SystemExceptionCreator {
    
  public:

   INV_OBJREF_Creator(){}
   virtual ~INV_OBJREF_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::INV_OBJREF(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::INV_OBJREF(dynamic_cast<const CORBA::INV_OBJREF&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
