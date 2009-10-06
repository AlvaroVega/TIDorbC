#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_SYSTEM_EXCEPTION_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_SYSTEM_EXCEPTION_CREATOR_H_

#include "CORBA.h"

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#define __STD_ISTREAM__
#define __STD_OSTREAM__
#endif

#include <map>

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#undef __STD_ISTREAM__
#undef __STD_OSTREAM__
#endif

#include <string>

#ifdef TIDORB_HAVE_NAMESPACE_STD
   using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
  using std::string;
#endif

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {

class SystemExceptionCreator {

  public:

  virtual ~SystemExceptionCreator(){}

  virtual CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus completion) = 0;
  virtual CORBA::SystemException* copy(const CORBA::SystemException& se) = 0;
  protected:
   SystemExceptionCreator(){}
 
};

typedef map<string, SystemExceptionCreator*> SystemExceptionCreatorTable;


} // exceptions
} // util
} // core
} // TIDorb

#endif 
