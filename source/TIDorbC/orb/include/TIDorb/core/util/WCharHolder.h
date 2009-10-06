#ifndef __TIDorb_core_util_WCharHolder_H_
#define __TIDorb_core_util_WCharHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {

/**
 * Particular Holder for WChar
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class WCharHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::WChar value;

    WCharHolder() {}
    
    WCharHolder(CORBA::WChar initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_wchar(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const {
        os.write_wchar(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_wchar);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_wchar);
    }

};

}
}
}

#endif
