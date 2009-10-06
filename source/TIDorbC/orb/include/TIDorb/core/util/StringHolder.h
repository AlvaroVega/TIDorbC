#ifndef __TIDorb_core_util_StringHolder_H_
#define __TIDorb_core_util_StringHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
  

/**
 * Particular Holder for String references
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class StringHolder : public TIDorb::portable::Streamable {

  public:
    
    StringHolder()
    {
      m_value = NULL;
      m_type = CORBA::TypeCode::_nil();
    }
    
    void value(char* initial, CORBA::TypeCode_ptr type = NULL)
    {
      reset_value();
      
      m_value = initial;
           
      if(!CORBA::is_nil(type))
        m_type = CORBA::TypeCode::_duplicate(type);
      else
        //m_type = CORBA::TypeCode::_duplicate(CORBA::_tc_string);
        m_type = TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_string);
    }
    
    void value(const char* initial, CORBA::TypeCode_ptr type = NULL)
    {
      reset_value();
      m_value = CORBA::string_dup(initial);
        
      if(!CORBA::is_nil(type))
        m_type = CORBA::TypeCode::_duplicate(type);
    }
    
    const char* value() const
    {
      return m_value;
    }
    
    
    ~StringHolder()
    {
      reset_value();
    }

    void _read(TIDorb::portable::InputStream& is) 
    {
      reset_value();        
      is.read_string(m_value);
     }

    void _write(TIDorb::portable::OutputStream& os) const 
    {
        os.write_string(m_value);
    }

    CORBA::TypeCode_ptr _type() const
    {
      if(!CORBA::is_nil(m_type))
        return CORBA::TypeCode::_duplicate(m_type);
      else
        return CORBA::TypeCode::_duplicate(CORBA::_tc_string);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_string);
    }
    
  protected:
  
    void reset_value()
    {
      if(m_value)
        CORBA::string_free(m_value);
        
      if(!CORBA::is_nil(m_type))
        CORBA::release(m_type);
        
      m_value = NULL;
      m_type = CORBA::TypeCode::_nil();
    }
    
    char* m_value;
    CORBA::TypeCode_ptr m_type;
 
};

}
}
}

#endif
