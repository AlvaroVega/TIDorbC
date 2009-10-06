#include "TIDorb/core/util.h"

#ifndef __TIDorb_core_util_Corbaname_H_
#define __TIDorb_core_util_Corbaname_H_

#include "CORBA.h"
#include "CosNaming.h"

namespace TIDorb {
namespace core {
namespace util {


class Corbaname {

  public:
  
    static CORBA::Object_ptr to_object(TIDorb::core::TIDORB* tidorb, 
                                       const char* corbaname_URL) 
      throw (CosNaming::NamingContext::InvalidName,
		         CosNaming::NamingContext::NotFound,
		         CosNaming::NamingContext::CannotProceed,
		         CORBA::ORB::InvalidName);
		         
    static CosNaming::Name* to_name(const char* sn) 
		  throw (CosNaming::NamingContext::InvalidName);
		
		static char* to_string(const CosNaming::Name* n);
	
	private:
	
		static CosNaming::NamingContext_ptr parse_protocol(TIDorb::core::TIDORB* tidorb, 
		                                                   const char* protocol)
		  throw (CosNaming::NamingContext::InvalidName,
		         CORBA::ORB::InvalidName);
  
};

}
}
}

#endif

