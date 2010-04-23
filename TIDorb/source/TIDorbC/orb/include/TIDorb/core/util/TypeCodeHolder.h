/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/core/util/TypeCodeHolder.h
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

#ifndef __TIDorb_core_util_TypeCodeHolder_H_
#define __TIDorb_core_util_TypeCodeHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {

/**
 * Particular Holder for TypeCode references
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class TypeCodeHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::TypeCode_ptr value;
 
    TypeCodeHolder()
    {
      value = NULL;
    }
    
    TypeCodeHolder(CORBA::TypeCode_ptr initial)
    {
      value = CORBA::TypeCode::_duplicate(initial);
    }
   
    ~TypeCodeHolder()
    {
      CORBA::release(value);
     }

    void _read(TIDorb::portable::InputStream& is) {
      if(value) 
        CORBA::release(value);
        
       is.read_TypeCode(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_TypeCode(value);
    }

    CORBA::TypeCode_ptr _type() const {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_TypeCode);
        //return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_TypeCode);
    }

};

}
}
}

#endif
