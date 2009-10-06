#ifndef __TIDorb_core_util_OctetHolder_H_
#define __TIDorb_core_util_OctetHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
  

/**
 * Particular Holder for Octet
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class OctetHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::Octet value;

    OctetHolder() {}
    
    OctetHolder(CORBA::Octet initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_octet(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_octet(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_octet);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_octet);
    }

};

}
}
}

#endif
