#include "CORBA.h" 

#ifndef _TIDORB_CORE_UTIL_FIXEDHOLDER_H_                                                  
#define _TIDORB_CORE_UTIL_FIXEDHOLDER_H_     

#include "TIDorb/portable/Streamable.h"

namespace TIDorb {
namespace core {
namespace util {
	
class FixedHolder: public ::TIDorb::portable::Streamable {

	public: // members
		CORBA::TypeCode_ptr fixed_type;
		CORBA::Fixed value;
	
	public: //methods
		FixedHolder(CORBA::TypeCode_ptr t_code);
		FixedHolder(CORBA::TypeCode_ptr t_code,const CORBA::Fixed& f);
		void _write(::TIDorb::portable::OutputStream& outs) const;		
		void _read(::TIDorb::portable::InputStream& ins); 
		::CORBA::TypeCode_ptr _type() const;
		

}; // END OF _FixedHolder		

} // end of namespace util
} // end of namespace core
} // end of namespace TIDorb

#endif
