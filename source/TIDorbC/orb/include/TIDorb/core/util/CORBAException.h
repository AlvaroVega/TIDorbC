#include "TIDorb/core/util.h"

#ifndef __TIDorb_core_util_CORBAException_H_
#define __TIDorb_core_util_CORBAException_H_

#include <exception>
#include <string>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

namespace TIDorb {
namespace core {
namespace util {

class CORBAException : public virtual exception {

  public:
    CORBAException(const CORBA::Exception& ex) throw() : _ex(ex) /*, mesg(NULL)*/ {}
    ~CORBAException() throw() {}

    const char* what() const throw();
    
  private:
    const CORBA::Exception& _ex;
    string mesg;
  
};

}
}
}

#endif
