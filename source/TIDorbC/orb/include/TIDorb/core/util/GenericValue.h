#include "TIDorb/core.h"
#include "TIDThr.h"

#ifndef _TIDORB_CORE_CDR_GENERICVALUE_H_
#define _TIDORB_CORE_CDR_GENERICVALUE_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {

/**
 *  Class that allows the Value remarshalling with anys
 *  This value must be locally constrained to the buffer where it was
 *  readed. 
 */
     
class GenericValue: public virtual CORBA::DefaultValueRefCountBase 
{
    public:
        GenericValue(const CORBA::TypeCode_ptr type)
          :  m_type(type){}
        
        GenericValue(const CORBA::TypeCode_ptr type,
                     const TIDorb::core::cdr::PointerCDR& position)
          :  m_type(type), m_marshaled_value(position)
        {}
          
        GenericValue(const GenericValue& value);
        
        ~GenericValue() {}
                
        CORBA::ValueBase* _copy_value()
        {
            return new GenericValue(*this);
        }
        
        // TIDorb marshalling methods
        void _read(TIDorb::portable::InputStream& input);
        void _write(TIDorb::portable::OutputStream& out) const;
        
        CORBA::TypeCode_ptr _type() const
        {
            return CORBA::TypeCode::_duplicate(m_type.in());
        }        
        
        
    protected:
    
        CORBA::TypeCode_var m_type;
        TIDorb::core::cdr::PointerCDR m_marshaled_value; 
        // TIDThr::Mutex m_mutex;
};

} // end namespaces
}
}

#endif //_TIDORB_CORE_CDR_GENERICVALUE_H_
