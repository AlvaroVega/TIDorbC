/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        AnyHolder.h
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
    bool free; 

    AnyHolder()
    {
      m_value = 0;
      free=true;
    }
    
    void value(CORBA::Any* initial)
    {
      if(free)
      {
        delete m_value; 
      }
      free=true;
      m_value = initial;
     }
    
    void value(const CORBA::Any& initial)
    {
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
      if(free)
        delete m_value;

    }

    void _read(TIDorb::portable::InputStream& is) 
    {
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
