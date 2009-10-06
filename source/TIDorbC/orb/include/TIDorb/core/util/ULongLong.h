#ifndef __TIDorb_core_util_ULongLong_H_
#define __TIDorb_core_util_ULongLong_H_

#include "CORBA.h"

#include <string>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::string;
#endif

namespace TIDorb {
namespace core {
namespace util {
  

class ULongLong {
  
  public:
    const static CORBA::ULongLong MAX_VALUE;
    static CORBA::ULongLong parseULongLong(const string& str);

};

} //util
} //core
} //TIDorb

#endif
