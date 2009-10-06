#ifndef __TIDorb_core_util_Double_H_
#define __TIDorb_core_util_Double_H_

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
  

class Double {

  public:
    const static CORBA::Double MIN_VALUE;
    const static CORBA::Double MAX_VALUE;
    static CORBA::Double parseDouble(const string& str);

};

} //util
} //core
} //TIDorb

#endif
