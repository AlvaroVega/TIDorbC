#ifndef __TIDorb_core_util_Short_H_
#define __TIDorb_core_util_Short_H_

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
  

class Short {

  public:
    const static CORBA::Short MIN_VALUE;
    const static CORBA::Short MAX_VALUE;
    static CORBA::Short parseShort(const string& str);

};

} //util
} //core
} //TIDorb

#endif
