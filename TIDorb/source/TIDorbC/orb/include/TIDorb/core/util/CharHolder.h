/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        CharHolder.h
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

#ifndef __TIDorb_core_util_CharHolder_H_
#define __TIDorb_core_util_CharHolder_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {

/**
 * Particular Holder for Char
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
class CharHolder : public TIDorb::portable::Streamable {

  public:
    
    CORBA::Char value;

    CharHolder() {}
    
    CharHolder(CORBA::Char initial) {
        value = initial;
    }

    void _read(TIDorb::portable::InputStream& is) {
       is.read_char(value);
    }

    void _write(TIDorb::portable::OutputStream& os) const {
        os.write_char(value);
    }

    CORBA::TypeCode_ptr _type() const
    {
        return CORBA::TypeCode::_duplicate(CORBA::_tc_char);
    }

};

}
}
}

#endif
