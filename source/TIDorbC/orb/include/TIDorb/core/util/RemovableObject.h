#include "TIDorb/core/util.h"

#ifndef _TIDORB_core_util_RemovableObject_H_
#define _TIDORB_core_util_RemovableObject_H_

namespace TIDorb {
	
namespace core {
	
namespace util {
	
class RemovableObject {

 public:  
  virtual bool canBeRemoved()=0;
  virtual ~RemovableObject() {}
};

}

}

}
#endif
