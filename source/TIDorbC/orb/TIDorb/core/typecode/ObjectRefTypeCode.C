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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

  //jagd
  //if(CORBA::is_nil(obj_a)){
  if(!(obj_a)){
    //jagd 
    //if(CORBA::is_nil(obj_b)) {
    if(!(obj_b)) {
      return true;
    } else {
      return false;
    }
  }

  //jagd
  //if(CORBA::is_nil(obj_b))
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
