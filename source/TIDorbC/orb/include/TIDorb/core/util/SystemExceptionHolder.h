#ifndef __TIDorb_core_util_SystemExceptionHolder_H_
#define __TIDorb_core_util_SystemExceptionHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {  


class SystemExceptionHolder : public TIDorb::portable::Streamable {

  public:

    SystemExceptionHolder()
    {
      m_value = 0;
    }
    
    SystemExceptionHolder(CORBA::SystemException* ex)
    {
      m_value = ex;
    }
    
    SystemExceptionHolder(const CORBA::SystemException& ex);

    SystemExceptionHolder(const char* rep_id);
        
    void value (CORBA::SystemException* ex)
    { 
      if(m_value != ex) {
          delete m_value;     
          m_value = ex;
      }
    }
    
    const CORBA::SystemException* value() const
    {
      return m_value;
    }
    
    ~SystemExceptionHolder()
    {
      delete m_value;
    }

    void _read(TIDorb::portable::InputStream& is); 
    

    void _write(TIDorb::portable::OutputStream& os) const;
    

    CORBA::TypeCode_ptr _type() const;
    

  protected:  
    CORBA::SystemException_ptr m_value;
  
   
};

}
}
}

#endif
