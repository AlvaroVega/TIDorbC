/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        StringHolder.h
// Description: 
//
// Rel:         01.00
// Created:     
// Author:      
//
// Revised:
//
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
//
///////////////////////////////////////////////////////////////////////////////////*/

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
      m_type = NULL;
    }
    
    void value(char* initial, CORBA::TypeCode_ptr type = NULL)
    {
      reset_value();
      
      m_value = initial;
           
      if(type)
        m_type = CORBA::TypeCode::_duplicate(type);
      else
        //m_type = CORBA::TypeCode::_duplicate(CORBA::_tc_string);
        m_type = TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_string);
    }
    
    void value(const char* initial, CORBA::TypeCode_ptr type = NULL)
    {
      reset_value();
      m_value = CORBA::string_dup(initial);
        
      if(type)
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
      if(m_type)
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
        
      if(m_type)
        CORBA::release(m_type);
        
      m_value = NULL;
      m_type = NULL;
    }
    
    char* m_value;
    CORBA::TypeCode_ptr m_type;
 
};

}
}
}

#endif
