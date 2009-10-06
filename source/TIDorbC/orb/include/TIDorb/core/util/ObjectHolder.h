#ifndef __TIDorb_core_util_ObjectHolder_H_
#define __TIDorb_core_util_ObjectHolder_H_

namespace TIDorb {
namespace core {
namespace util {
  
#include "CORBA.h"

/**
 * Particular Holder for Object references
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class ObjectHolder : public TIDorb::portable::Streamable {

  public:

    ObjectHolder()
    {
      m_value = CORBA::Object::_nil();
      m_reference_type = CORBA::TypeCode::_duplicate(CORBA::_tc_Object);
      //m_reference_type = TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_objref);
    }
    
    void value (CORBA::Object_ptr initial)
    {
      reset_value();
      m_value = CORBA::Object::_duplicate(initial);
      m_reference_type = CORBA::TypeCode::_duplicate(CORBA::_tc_Object);        
      //m_reference_type = TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_objref);
    }
    
    void value (CORBA::Object_ptr initial, const CORBA::TypeCode_ptr type) 
    {
      reset_value();
      m_value = CORBA::Object::_duplicate(initial);
      m_reference_type = CORBA::TypeCode::_duplicate(type);        
    }
    
    CORBA::Object_ptr value()
    {
      return CORBA::Object::_duplicate(m_value);
    }
    
    ~ObjectHolder()
    {
      reset_value();
    }


    void _read(TIDorb::portable::InputStream& is) 
    {
       reset_value();       
       m_reference_type = CORBA::TypeCode::_duplicate(CORBA::_tc_Object);
       is.read_Object(m_value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_Object(m_value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(m_reference_type);
    }

  protected:
    
    void reset_value()
    {
      CORBA::release(m_value);
      CORBA::release(m_reference_type);
    }
  
   CORBA::Object_ptr m_value;
   CORBA::TypeCode_ptr m_reference_type;
   
};

}
}
}

#endif
