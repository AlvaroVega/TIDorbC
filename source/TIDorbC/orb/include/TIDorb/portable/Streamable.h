/*/////////////////////////////////////////////////////////////////////////
//
// File:        Streamable.h
// Description: 
//              
// Rel:         01.00
// Created:     November, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
/////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/portable.h"

#ifndef _TIDORB_PORTABLE_STREAMABLE_H_
#define _TIDORB_PORTABLE_STREAMABLE_H_ 


namespace TIDorb {
namespace portable {

class Streamable : public virtual TIDorb::portable::RefCounter {

  public:


    virtual ~Streamable() {}
    
    virtual void _read(InputStream& input) = 0;
    virtual void _write(OutputStream& out) const = 0;
    virtual CORBA::TypeCode_ptr _type() const = 0;

  protected:
    Streamable() {_count(1);}
};
  
}
}

#endif
