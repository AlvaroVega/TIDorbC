/////////////////////////////////////////////////////////////////////////
//
// File:        ValueBoxTypeCode.C
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
#include <string.h>


namespace TIDorb {
namespace core {
namespace typecode {

ValueBoxTypeCode::ValueBoxTypeCode()
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_value_box), m_boxed_type(0)
{
}




ValueBoxTypeCode::ValueBoxTypeCode (const char* id, 
                                    const char* name, 
                                    ::CORBA::TypeCode_ptr boxed_type)
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_value_box, id, name)
{
  m_boxed_type = CORBA::TypeCode::_duplicate(boxed_type);
}




ValueBoxTypeCode::~ValueBoxTypeCode()
  throw (TIDThr::SystemException)
{
  CORBA::release(m_boxed_type);
}




CORBA::Boolean ValueBoxTypeCode::equal(CORBA::TypeCode_ptr tc) const
{
  if (!ComplexTypeCode::equal(tc))
        return false;

  if (!m_exhaustive_equal)
        return true;

  ValueBoxTypeCode*  value_tc = dynamic_cast<ValueBoxTypeCode*>(tc);

  if(!value_tc)
    return false;

   return m_boxed_type->equal(value_tc->m_boxed_type);
}

CORBA::Boolean ValueBoxTypeCode::equivalent(CORBA::TypeCode_ptr tc) const
{
  if (!TypeCodeImpl::equivalent(tc))
        return false;

  if (!m_exhaustive_equal)
        return true;

  ValueBoxTypeCode*  value_tc = dynamic_cast<ValueBoxTypeCode*>(tc);

  if(!value_tc)
    return false;

   return m_boxed_type->equivalent(value_tc->m_boxed_type);
}



CORBA::TypeCode_ptr ValueBoxTypeCode::content_type() const
{
    return CORBA::TypeCode::_duplicate(m_boxed_type);
}




void ValueBoxTypeCode::write_params
      (TIDorb::core::cdr::CDROutputStream& output)
{
  ComplexTypeCode::write_params(output);
  output.write_TypeCode(m_boxed_type);
}




void ValueBoxTypeCode::read_params
       (TIDorb::core::cdr::CDRInputStream& input)
{
  ComplexTypeCode::read_params(input);
//FRAN
  if(m_boxed_type){
    CORBA::release(m_boxed_type);
  }
//EFRAN
  input.read_TypeCode(m_boxed_type);
}




bool ValueBoxTypeCode::skip_value
  (TIDorb::core::cdr::CDRInputStream& input) const
{
  TypeCodeImpl* tc = 
  //dynamic_cast<TypeCodeImpl *> (m_boxed_type);
  //jagd 
  //(TypeCodeImpl*)m_boxed_type->_impl();
  (TypeCodeImpl*)m_boxed_type;
  
  return tc->skip_value(input);
}




void ValueBoxTypeCode::remarshal_value
       (TIDorb::core::cdr::CDRInputStream& input,
        TIDorb::core::cdr::CDROutputStream& output) const
{
    TypeCodeImpl* tc = 
    //dynamic_cast<TypeCodeImpl *> (m_boxed_type);
    //jagd 
    //(TypeCodeImpl*)m_boxed_type->_impl();    
    (TypeCodeImpl*)m_boxed_type;    
    
    tc->remarshal_value(input, output);
}




bool ValueBoxTypeCode::values_equal
       (TIDorb::core::cdr::CDRInputStream& a_input,
        TIDorb::core::cdr::CDRInputStream& b_input) const
{
  TypeCodeImpl* tc = 
  //dynamic_cast<TypeCodeImpl *> (m_boxed_type);
  //jagd 
  //(TypeCodeImpl*)m_boxed_type->_impl();
  (TypeCodeImpl*)m_boxed_type;

  return tc->values_equal(a_input, b_input);
}




void ValueBoxTypeCode::dump (ostream& output) const
{

  output << "[TYPECODE]{valuebox boxed type=";
  TypeCodeImpl* tc = 
  //dynamic_cast<TypeCodeImpl*>(m_boxed_type);
  //jagd
  //(TypeCodeImpl*)m_boxed_type->_impl();
  (TypeCodeImpl*)m_boxed_type;
  
  tc->dump(output);
   output << '}';
   output << flush;
}




bool ValueBoxTypeCode::dump_value
       (TIDorb::core::cdr::CDRInputStream& input, ostream& output) const
{

  output << "[VALUE]{valuebox " << m_name << " : ";

  TypeCodeImpl* tc =
  //dynamic_cast<TypeCodeImpl*>(m_boxed_type);
  //jagd 
  //(TypeCodeImpl*)m_boxed_type->_impl();
  (TypeCodeImpl*)m_boxed_type;
  
  if(!tc->dump_value(input, output))
      return false;

  output << " END_VALUE-BOX}";

  return true;
}

} // namespace TIDorb/core/typecode
}
}
