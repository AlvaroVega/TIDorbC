#ifndef __TIDorb_core_util_ShortHolder_H_
#define __TIDorb_core_util_ShortHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {

/**
 * Particular Holder for Octet
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class ShortHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::Short value;

    ShortHolder() {}
    
    ShortHolder(CORBA::Short initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_short(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_short(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_short);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_short);
    }

};

}
}
}

#endif
