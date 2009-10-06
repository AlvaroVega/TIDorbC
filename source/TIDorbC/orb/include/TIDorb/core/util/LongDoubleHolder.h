#ifndef __TIDorb_core_util_LongDoubleHolder_H_
#define __TIDorb_core_util_LongDoubleHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {

/**
 * Particular Holder for LongDouble
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class LongDoubleHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::LongDouble value;

    LongDoubleHolder() {}
    
    LongDoubleHolder(CORBA::LongDouble initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_longdouble(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_longdouble(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_longdouble);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_longdouble);
    }

};

}
}
}

#endif
