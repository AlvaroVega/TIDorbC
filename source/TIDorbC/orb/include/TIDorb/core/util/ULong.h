#ifndef __TIDorb_core_util_ULong_H_
#define __TIDorb_core_util_ULong_H_

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
  

class ULong {

  public:
    const static CORBA::ULong MAX_VALUE;
    static CORBA::ULong parseULong(const string& str);

};

} //util
} //core
} //TIDorb

#endif
