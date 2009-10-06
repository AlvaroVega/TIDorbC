#ifndef __TIDorb_core_util_Long_H_
#define __TIDorb_core_util_Long_H_

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
  

class Long {

  public:
    const static CORBA::Long MIN_VALUE;
    const static CORBA::Long MAX_VALUE;
    static CORBA::Long parseLong(const string& str);

};

} //util
} //core
} //TIDorb

#endif
