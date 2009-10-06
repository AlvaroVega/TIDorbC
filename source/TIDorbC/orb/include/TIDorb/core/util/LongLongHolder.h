#ifndef __TIDorb_core_util_LongLongHolder_H_
#define __TIDorb_core_util_LongLongHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {


/**
 * Particular Holder for Octet
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class LongLongHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::LongLong value;

    LongLongHolder() {}
    
    LongLongHolder(CORBA::LongLong initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_longlong(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_longlong(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_longlong);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_longlong);
    }

};

}
}
}

#endif

