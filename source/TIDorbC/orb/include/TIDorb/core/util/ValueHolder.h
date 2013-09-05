/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        ValueHolder.h
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

#ifndef __TIDorb_core_util_ValueHolder_H_
#define __TIDorb_core_util_ValueHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {  

/**
 * Particular Holder for Value references
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class ValueHolder : public TIDorb::portable::Streamable {

  public:

    ValueHolder()
    {
      m_value = 0;
    }
    
    void value (CORBA::ValueBase* initial)
    {
      reset_value();
      
      if(initial) {
          m_value = initial->_add_ref();
      }
    }
    
    CORBA::ValueBase* value()
    {
       if(m_value) {
        return m_value->_add_ref();
       } else {
        return NULL;
       }
    }
    
    ~ValueHolder()
    {
      reset_value();
    }

    void _read(TIDorb::portable::InputStream& is) 
    {
       reset_value();       
       is.read_Value(m_value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
       os.write_Value(m_value);
    }

    CORBA::TypeCode_ptr _type() const {
        if (m_value == 0) {
           return CORBA::TypeCode::_duplicate(CORBA::_tc_Value);
        } else {
           return m_value->_type();
        }
    }

  protected:
    
    void reset_value()
    {
        if(m_value) {
           m_value->_remove_ref();
        }
        m_value = NULL;
    }
  
    CORBA::ValueBase* m_value;
   
};

}
}
}

#endif
