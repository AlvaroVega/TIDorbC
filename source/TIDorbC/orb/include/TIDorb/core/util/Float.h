#ifndef __TIDorb_core_util_Float_H_
#define __TIDorb_core_util_Float_H_

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
  

class Float {

  public:
    const static CORBA::Float MIN_VALUE;
    const static CORBA::Float MAX_VALUE;
    static CORBA::Float parseFloat(const string& str);

};

} //util
} //core
} //TIDorb

#endif
