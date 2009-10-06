#ifndef __TIDorb_core_util_LongLong_H_
#define __TIDorb_core_util_LongLong_H_

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
  

class LongLong {
  
  public:
    const static CORBA::LongLong MIN_VALUE;
    const static CORBA::LongLong MAX_VALUE;
    static CORBA::LongLong parseLongLong(const string& str);

};

} //util
} //core
} //TIDorb

#endif
