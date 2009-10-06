#ifndef __TIDorb_core_util_ULongHolder_H_
#define __TIDorb_core_util_ULongHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
  
/**
 * Particular Holder for Octet
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class ULongHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::ULong value;

    ULongHolder() {}
    
    ULongHolder(CORBA::ULong initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_ulong(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_ulong(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_ulong);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_ulong);
    }

};

}
}
}

#endif

