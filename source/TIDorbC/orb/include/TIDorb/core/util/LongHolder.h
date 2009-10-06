#ifndef __TIDorb_core_util_LongHolder_H_
#define __TIDorb_core_util_LongHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
  

/**
 * Particular Holder for Long
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class LongHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::Long value;

    LongHolder() {}
    
    LongHolder(CORBA::Long initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_long(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const {
        os.write_long(value);
    }

    CORBA::TypeCode_ptr _type() const {
        //return CORBA::TypeCode::_duplicate(CORBA::_tc_long);
        return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_long);
    }

};

}
}
}

#endif

