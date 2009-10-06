#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_SYSTEM_EXCEPTION_TYPE_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_SYSTEM_EXCEPTION_TYPE_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {

class SystemExceptionType {

  public:  

  static CORBA::TypeCode_ptr create(const char* id);
  
 
};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
