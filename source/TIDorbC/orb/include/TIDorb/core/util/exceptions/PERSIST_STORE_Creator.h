#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_PERSIST_STORE_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_PERSIST_STORE_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class PERSIST_STORE_Creator : public virtual SystemExceptionCreator {
    
  public:

   PERSIST_STORE_Creator(){}
   virtual ~PERSIST_STORE_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::PERSIST_STORE(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::PERSIST_STORE(dynamic_cast<const CORBA::PERSIST_STORE&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
