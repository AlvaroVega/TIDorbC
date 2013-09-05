/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        ObjectHolder.h
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
      m_value = NULL;
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
