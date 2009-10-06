#ifndef __TIDorb_core_util_CharHolder_H_
#define __TIDorb_core_util_CharHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {

/**
 * Particular Holder for Char
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class CharHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::Char value;

    CharHolder() {}
    
    CharHolder(CORBA::Char initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_char(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const {
        os.write_char(value);
    }

    CORBA::TypeCode_ptr _type() const
    {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_char);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_char);
    }

};

}
}
}

#endif
