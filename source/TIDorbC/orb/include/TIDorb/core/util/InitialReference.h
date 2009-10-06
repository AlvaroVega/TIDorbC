#ifndef __TIDorb_core_util_InitialReference_H_
#define __TIDorb_core_util_InitialReference_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {


class InitialReference {

  public:
    
    InitialReference();
    InitialReference(const char* name, const char* url);
    InitialReference(const InitialReference& v);
    
    const char* get_name() const
    {
      return object_name;
    }
    
    const char* get_url() const
    {
	    return object_url;  
    }
    
    void parse(const char* value) throw(CORBA::SystemException);
    

  protected:
  
    CORBA::String_var object_name;
    CORBA::String_var object_url;       
};

}
}
}

#endif
