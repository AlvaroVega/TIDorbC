#ifndef __TIDorb_core_util_UShortHolder_H_
#define __TIDorb_core_util_UShortHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
  
/**
 * Particular Holder for Octet
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class UShortHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::UShort value;

    UShortHolder() {}
    
    UShortHolder(CORBA::UShort initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_ushort(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_ushort(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_ushort);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_ushort);
    }

};

}
}
}

#endif
