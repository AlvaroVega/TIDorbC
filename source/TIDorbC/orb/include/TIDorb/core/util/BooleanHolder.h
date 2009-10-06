#ifndef __TIDorb_core_util_BooleanHolder_H_
#define __TIDorb_core_util_BooleanHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
  
/**
 * Particular Holder for Boolean
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class BooleanHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::Boolean value;

    BooleanHolder() {}
    
    BooleanHolder(CORBA::Boolean initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_boolean(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_boolean(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_boolean);
	//return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(CORBA::tk_boolean);
    }

};

}
}
}

#endif
