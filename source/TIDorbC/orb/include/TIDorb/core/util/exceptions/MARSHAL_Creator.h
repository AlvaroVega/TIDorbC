#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_MARSHAL_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_MARSHAL_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class MARSHAL_Creator : public virtual SystemExceptionCreator {
    
  public:

   MARSHAL_Creator(){}
   virtual ~MARSHAL_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::MARSHAL(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::MARSHAL(dynamic_cast<const CORBA::MARSHAL&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
