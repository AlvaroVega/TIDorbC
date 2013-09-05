/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        ULongHolder.h
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

#ifndef __TIDorb_core_util_ULongHolder_H_
#define __TIDorb_core_util_ULongHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
  
/**
 * Particular Holder for Octet
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class ULongHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::ULong value;

    ULongHolder() {}
    
    ULongHolder(CORBA::ULong initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_ulong(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const
    {
        os.write_ulong(value);
    }

    CORBA::TypeCode_ptr _type() const {
      //return CORBA::TypeCode::_duplicate(CORBA::_tc_ulong);
        // Temporal workarround of core dump at static initialization due to unions at CSI.idl             
        return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_ulong);
    }

};

}
}
}

#endif

