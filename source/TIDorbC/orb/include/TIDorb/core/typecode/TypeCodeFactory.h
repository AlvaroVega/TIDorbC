#include "TIDorb/core/typecode.h"

#ifndef _TIDorb_core_typecode_TypeCodeFactory_H_
#define _TIDorb_core_typecode_TypeCodeFactory_H_

namespace TIDorb {
namespace core {
namespace typecode {
  
class TypeCodeFactory {
	
  public:
    //static CORBA::TypeCode_ptr get_basic_TypeCode(CORBA::TCKind tcKind);
    static TypeCodeImpl* get_empty_TypeCode(CORBA::TCKind tcKind);
};

}
}
}


#endif
