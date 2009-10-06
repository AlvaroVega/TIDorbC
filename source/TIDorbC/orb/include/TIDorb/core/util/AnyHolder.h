#ifndef __TIDorb_core_util_AnyHolder_H_
#define __TIDorb_core_util_AnyHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {

/**
 * Particular Holder for Any references
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class AnyHolder : public TIDorb::portable::Streamable {

  public:
    //jagd
    bool free; 

    AnyHolder()
    {
      m_value = 0;
      //jagd 
      free=true;
    }
    
    void value(CORBA::Any* initial)
    {
      //jagd reset_value();
      if(free)
      {
        delete m_value; 
      }
      free=true;
      m_value = initial;
     }
    
    void value(const CORBA::Any& initial)
    {
      //jagd reset_value();
      if(free)
      {
        delete m_value; 
      }
      free=true;
      m_value = new CORBA::Any();
      *m_value = initial;
     }
    
    const CORBA::Any* value() const
    {
      return m_value;
    }
    
    
    ~AnyHolder()
    {
      //jagd reset_value();
      if(free)
        delete m_value;

    }

    void _read(TIDorb::portable::InputStream& is) 
    {
      //jagd reset_value();
      if(free) 
        delete m_value;
      free=true;
      m_value = new CORBA::Any;
      is.read_any(*m_value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_any(*m_value);
    }

    CORBA::TypeCode_ptr _type() const {
      //return CORBA::TypeCode::_duplicate(CORBA::_tc_any);
      return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_any);
    }
    
  protected:
  
    void reset_value()
    {
      if(m_value)
        delete m_value;
      m_value = 0;
     }
    
    CORBA::Any* m_value;
};

}
}
}

#endif
