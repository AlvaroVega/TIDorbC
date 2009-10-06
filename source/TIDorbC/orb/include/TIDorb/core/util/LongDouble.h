#ifndef __TIDorb_core_util_LongDouble_H_
#define __TIDorb_core_util_LongDouble_H_

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
  

class LongDouble {

  public:
    const static CORBA::LongDouble MIN_VALUE;
    const static CORBA::LongDouble MAX_VALUE;
    static CORBA::LongDouble parseLongDouble(const string& str);

};

} //util
} //core
} //TIDorb

#endif
