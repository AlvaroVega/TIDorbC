#ifndef __TIDorb_core_util_TypeCodeHolder_H_
#define __TIDorb_core_util_TypeCodeHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {

/**
 * Particular Holder for TypeCode references
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class TypeCodeHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::TypeCode_ptr value;
 
    TypeCodeHolder()
    {
      value = CORBA::TypeCode::_nil();
    }
    
    TypeCodeHolder(CORBA::TypeCode_ptr initial)
    {
      value = CORBA::TypeCode::_duplicate(initial);
    }
   
    ~TypeCodeHolder()
    {
      CORBA::release(value);
     }

    void _read(TIDorb::portable::InputStream& is) {
      if(!CORBA::is_nil(value)) 
        CORBA::release(value);
        
       is.read_TypeCode(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_TypeCode(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_TypeCode);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_TypeCode);
    }

};

}
}
}

#endif
