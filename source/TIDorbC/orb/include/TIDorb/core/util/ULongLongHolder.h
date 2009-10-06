#ifndef __TIDorb_core_util_ULongLongHolder_H_
#define __TIDorb_core_util_ULongLongHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {  

/**
 * Particular Holder for Octet
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class ULongLongHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::ULongLong value;

    ULongLongHolder() {}
    
    ULongLongHolder(CORBA::ULongLong initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_ulonglong(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_ulonglong(value);
    }

    CORBA::TypeCode_ptr _type() const  {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_ulonglong);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_ulonglong);
    }

};

}
}
}

#endif

