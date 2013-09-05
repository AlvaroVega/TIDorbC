/////////////////////////////////////////////////////////////////////////
//
// File:        AbstractInterfaceTypeCode.h
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




TIDorb::core::typecode::AbstractInterfaceTypeCode::AbstractInterfaceTypeCode()
 throw (TIDThr::SystemException)
 : ComplexTypeCode(CORBA::tk_abstract_interface)
{
}




TIDorb::core::typecode::AbstractInterfaceTypeCode::AbstractInterfaceTypeCode
  (const char* id, const char* name)
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_abstract_interface, id, name)
{
}




TIDorb::core::typecode::AbstractInterfaceTypeCode::~AbstractInterfaceTypeCode()
  throw (TIDThr::SystemException)
{
}




bool TIDorb::core::typecode::AbstractInterfaceTypeCode::skip_value
  (TIDorb::core::cdr::CDRInputStream& input) const
  throw (CORBA::SystemException)
{
  CORBA::AbstractBase_var abstract;
  input.read_Abstract(abstract.out());
  return true;
}




void TIDorb::core::typecode::AbstractInterfaceTypeCode::remarshal_value
  (TIDorb::core::cdr::CDRInputStream& input,
   TIDorb::core::cdr::CDROutputStream& output) const
  throw (CORBA::SystemException)
{
  CORBA::AbstractBase_var value;
  input.read_Abstract(value.out());
  output.write_Abstract((const CORBA::AbstractBase_ptr) value);
}




bool TIDorb::core::typecode::AbstractInterfaceTypeCode::values_equal
  (TIDorb::core::cdr::CDRInputStream& a_input,
   TIDorb::core::cdr::CDRInputStream& b_input) const throw (CORBA::SystemException)
{
  throw CORBA::NO_IMPLEMENT();
}




void TIDorb::core::typecode::AbstractInterfaceTypeCode::dump (ostream& output) const
  throw (CORBA::SystemException)
{
  output << "[TYPECODE]{abstract interface:";
  ComplexTypeCode::dump_params(output);
  output << '}';
}




bool TIDorb::core::typecode::AbstractInterfaceTypeCode::dump_value
  (TIDorb::core::cdr::CDRInputStream& input,
   ostream& output) const
  throw (CORBA::SystemException)
{
  throw CORBA::NO_IMPLEMENT();
}


