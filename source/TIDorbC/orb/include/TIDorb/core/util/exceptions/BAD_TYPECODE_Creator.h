#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_BAD_TYPECODE_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_BAD_TYPECODE_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class BAD_TYPECODE_Creator : public virtual SystemExceptionCreator {
    
  public:

   BAD_TYPECODE_Creator(){}
   virtual ~BAD_TYPECODE_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::BAD_TYPECODE(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::BAD_TYPECODE(dynamic_cast<const CORBA::BAD_TYPECODE&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
