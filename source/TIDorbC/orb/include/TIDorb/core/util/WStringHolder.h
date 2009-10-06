#ifndef __TIDorb_core_util_WStringHolder_H_
#define __TIDorb_core_util_WStringHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
 

/**
 * Particular Holder for WString references
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class WStringHolder : public TIDorb::portable::Streamable {

  public:
    
    WStringHolder()
    {
      m_value = 0;
      m_type = CORBA::TypeCode::_nil();
    }
    
    void value(CORBA::WChar* initial, CORBA::TypeCode_ptr type = 0)
    {
      reset_value();
      
      m_value = initial;
           
      if(!CORBA::is_nil(type))
        m_type = CORBA::TypeCode::_duplicate(type);
      else
        //m_type = CORBA::TypeCode::_duplicate(CORBA::_tc_wstring);
        m_type = TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_wstring);
    }
    
    void value(const CORBA::WChar* initial, CORBA::TypeCode_ptr type = 0)
    {
      reset_value();
      m_value = CORBA::wstring_dup(initial);
       
      if(!CORBA::is_nil(type))
        m_type = CORBA::TypeCode::_duplicate(type);
    }
    
    CORBA::WChar* value() const
    {
      return m_value;
    }
    
    
    ~WStringHolder()
    {
      reset_value();
    }

    void _read(TIDorb::portable::InputStream& is) 
    {
      reset_value();        
      is.read_wstring(m_value);
     }

     void _write(TIDorb::portable::OutputStream& os) const 
    {
        os.write_wstring(m_value);
    }

    CORBA::TypeCode_ptr _type() const
    {
      if(!CORBA::is_nil(m_type))
        return CORBA::TypeCode::_duplicate(m_type);
      else
        return CORBA::TypeCode::_duplicate(CORBA::_tc_wstring);
	//return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_wstring);
    }
    
  protected:
  
    void reset_value()
    {
      if(m_value)
        CORBA::wstring_free(m_value);
        
      if(!CORBA::is_nil(m_type))
        CORBA::release(m_type);
        
      m_value = 0;
      m_type = CORBA::TypeCode::_nil();
    }
    
    CORBA::WChar* m_value;
    CORBA::TypeCode_ptr m_type;

};

}
}
}

#endif
