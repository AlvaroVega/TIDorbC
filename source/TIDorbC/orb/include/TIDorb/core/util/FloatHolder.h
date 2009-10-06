#ifndef __TIDorb_core_util_FloatHolder_H_
#define __TIDorb_core_util_FloatHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
  

/**
 * Particular Holder for Float
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class FloatHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::Float value;

    FloatHolder() {}
    
    FloatHolder(CORBA::Float initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_float(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_float(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_float);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_float);
    }

};

}
}
}

#endif
