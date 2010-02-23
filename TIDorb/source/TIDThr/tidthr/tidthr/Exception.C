///////////////////////////////////////////////////////////////////////////
//
// File:        Exception.C
// Description: This file contains the Exception class methods
// Rel:         01.00
// Created:     May, 2001
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
///////////////////////////////////////////////////////////////////////////

#include "TIDThr.h"

#include <string.h>

namespace TIDThr {
  
Exception::Exception(const char* msg, int error)
    : m_reason(msg), m_errno(error) 
{ 
    if (strncmp(msg,"",1)!=0) {
        m_what = msg + string(" ");
    }

    if (error != 0) {
        m_what += string(strerror(error)) + string(" ") + string("[errno=");
        m_what += string(enostr(error)) + string("]");
    }  
}
  
} // namespace TIDThr



ostream& 
operator<< (ostream& os, const TIDThr::Exception& ex)
{
    os << ex.getName() << ": " << ex.what() << endl;
    return os;
};
