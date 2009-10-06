#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_INTF_REPOS_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_INTF_REPOS_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class INTF_REPOS_Creator : public virtual SystemExceptionCreator {
    
  public:

   INTF_REPOS_Creator(){}
   virtual ~INTF_REPOS_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::INTF_REPOS(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::INTF_REPOS(dynamic_cast<const CORBA::INTF_REPOS&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
