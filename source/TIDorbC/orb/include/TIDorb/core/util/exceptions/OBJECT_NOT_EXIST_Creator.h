#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_OBJECT_NOT_EXIST_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_OBJECT_NOT_EXIST_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class OBJECT_NOT_EXIST_Creator : public virtual SystemExceptionCreator {
    
  public:

   OBJECT_NOT_EXIST_Creator(){}
   virtual ~OBJECT_NOT_EXIST_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::OBJECT_NOT_EXIST(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::OBJECT_NOT_EXIST(dynamic_cast<const CORBA::OBJECT_NOT_EXIST&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
