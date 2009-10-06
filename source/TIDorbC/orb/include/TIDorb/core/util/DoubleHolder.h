#ifndef __TIDorb_core_util_DoubleHolder_H_
#define __TIDorb_core_util_DoubleHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
  


/**
 * Particular Holder for Double
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class DoubleHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::Double value;

    DoubleHolder() {}
    
    DoubleHolder(CORBA::Double initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_double(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_double(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_double);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_double); 
    }

};

}
}
}

#endif
