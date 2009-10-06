#include "TIDorb/core/typecode.h"

#ifndef _TIDorb_core_typecode_TypeCodeMarshaler_H_
#define _TIDorb_core_typecode_TypeCodeMarshaler_H_

#include "CORBA.h"
#include "TIDThr.h"


namespace TIDorb {
namespace core {
namespace typecode { 

class TypeCodeMarshaler {
  
  public:
    static bool skip_value_array(CORBA::TypeCode_ptr type, 
                                 TIDorb::core::cdr::CDRInputStream& input,
                                 CORBA::ULong n);
};

}
}
}

#endif
