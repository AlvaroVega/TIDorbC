/////////////////////////////////////////////////////////////////////////
//
// File:        ObjectRefTypeCode.C
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




TIDorb::core::typecode::ObjectRefTypeCode::ObjectRefTypeCode()
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_objref)
{
}




TIDorb::core::typecode::ObjectRefTypeCode::ObjectRefTypeCode(const char* id, const char* name)
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_objref, id, name)
{
}




TIDorb::core::typecode::ObjectRefTypeCode::~ObjectRefTypeCode()
  throw (TIDThr::SystemException)
{
}




bool TIDorb::core::typecode::ObjectRefTypeCode::skip_value
  (TIDorb::core::cdr::CDRInputStream& input) const
{
  TIDorb::core::iop::IOR::skip(input);
  return true;
}




void TIDorb::core::typecode::ObjectRefTypeCode::remarshal_value
  (TIDorb::core::cdr::CDRInputStream& input,
   TIDorb::core::cdr::CDROutputStream& output) const
{
  CORBA::Object_var obj;
  input.read_Object(obj);
  output.write_Object(obj);
}




bool TIDorb::core::typecode::ObjectRefTypeCode::values_equal
  (TIDorb::core::cdr::CDRInputStream& a_input,
   TIDorb::core::cdr::CDRInputStream& b_input) const
{
  CORBA::Object_var obj_a, obj_b;

  a_input.read_Object(obj_a);
  b_input.read_Object(obj_b);

  if(!(obj_a)){
    if(!(obj_b)) {
      return true;
    } else {
      return false;
    }
  }

  if(!(obj_b))
    return false;

  return obj_a->_is_equivalent(obj_b);

}




void TIDorb::core::typecode::ObjectRefTypeCode::dump (ostream& output) const
{
  output << "[TYPECODE]{obj_reference:";
  ComplexTypeCode::dump_params(output);
  output << '}';
}




bool TIDorb::core::typecode::ObjectRefTypeCode::dump_value
  (TIDorb::core::cdr::CDRInputStream& input, ostream& output) const
{
  throw CORBA::NO_IMPLEMENT();
}
