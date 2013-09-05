/////////////////////////////////////////////////////////////////////////
//
// File:        LocalInterfaceTypeCode.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/typecode.h"




TIDorb::core::typecode::LocalInterfaceTypeCode::LocalInterfaceTypeCode()
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_local_interface)
{
}




TIDorb::core::typecode::LocalInterfaceTypeCode::LocalInterfaceTypeCode
  (const char* id, const char* name)
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_local_interface, id, name)
{
}




TIDorb::core::typecode::LocalInterfaceTypeCode::~LocalInterfaceTypeCode()
  throw (TIDThr::SystemException)
{
}




bool TIDorb::core::typecode::LocalInterfaceTypeCode::skip_value
  (TIDorb::core::cdr::CDRInputStream& input) const
{
  throw CORBA::NO_IMPLEMENT();
}




void TIDorb::core::typecode::LocalInterfaceTypeCode::remarshal_value
  (TIDorb::core::cdr::CDRInputStream& input,
   TIDorb::core::cdr::CDROutputStream& output) const
{
  throw CORBA::NO_IMPLEMENT();
}




bool TIDorb::core::typecode::LocalInterfaceTypeCode::values_equal
  (TIDorb::core::cdr::CDRInputStream& a_input,
   TIDorb::core::cdr::CDRInputStream& b_input) const
{
  throw CORBA::NO_IMPLEMENT();
}




void TIDorb::core::typecode::LocalInterfaceTypeCode::dump (ostream& output) const
{
  throw CORBA::NO_IMPLEMENT();
}




bool TIDorb::core::typecode::LocalInterfaceTypeCode::dump_value
  (TIDorb::core::cdr::CDRInputStream& input, ostream& output) const
{
  throw CORBA::NO_IMPLEMENT();
}
