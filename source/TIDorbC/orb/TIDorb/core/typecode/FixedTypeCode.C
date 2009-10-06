/////////////////////////////////////////////////////////////////////////
//
// File:        FixedTypeCode.C
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




TIDorb::core::typecode::FixedTypeCode::FixedTypeCode()
  throw (TIDThr::SystemException)
  : TypeCodeImpl(CORBA::tk_fixed), m_digits(0), m_scale(0)
{
}




TIDorb::core::typecode::FixedTypeCode::FixedTypeCode(CORBA::UShort digits, CORBA::Short scale)
  throw (TIDThr::SystemException)
  : TypeCodeImpl(CORBA::tk_fixed), m_digits(digits), m_scale(scale)
{
}




TIDorb::core::typecode::FixedTypeCode::~FixedTypeCode()
  throw (TIDThr::SystemException)
{
}




CORBA::Boolean TIDorb::core::typecode::FixedTypeCode::equal(CORBA::TypeCode_ptr tc) const
{
        if(!TypeCodeImpl::equal(tc))
                return false;
        try {
                return  (m_digits == tc->fixed_digits()) &&
                                                (m_scale == tc->fixed_scale());
        } catch (const CORBA::TypeCode::BadKind& e) {
                return false;
        }
}

CORBA::Boolean TIDorb::core::typecode::FixedTypeCode::equivalent(CORBA::TypeCode_ptr tc) const
{
        if(!TypeCodeImpl::equivalent(tc))
                return false;
        try {
                return  (m_digits == tc->fixed_digits()) &&
                                                (m_scale == tc->fixed_scale());
        } catch (const CORBA::TypeCode::BadKind& e) {
                return false;
        }
}



CORBA::UShort TIDorb::core::typecode::FixedTypeCode::fixed_digits() const
{
  return m_digits;
}




CORBA::Short TIDorb::core::typecode::FixedTypeCode::fixed_scale() const
{
  return m_scale;
}




bool TIDorb::core::typecode::FixedTypeCode::isSimple() const
{
  return false;
}




CORBA::ULong TIDorb::core::typecode::FixedTypeCode::value_length(CORBA::UShort digits)
{
    CORBA::ULong fix_digits = digits;

    return ( fix_digits + (fix_digits % 2))/2; //it must have a odd number of octets
}




void TIDorb::core::typecode::FixedTypeCode::partial_unmarshal(TIDorb::core::cdr::CDRInputStream& input)
{
  input.read_ushort(m_digits);
  input.read_short(m_scale);
}




void TIDorb::core::typecode::FixedTypeCode::marshal(TIDorb::core::cdr::CDROutputStream& output)
{
  output.write_long(m_kind);
  output.write_ushort(m_digits);
  output.write_short(m_scale);

}




void TIDorb::core::typecode::FixedTypeCode::remarshal_value
      (TIDorb::core::cdr::CDRInputStream& input,
       TIDorb::core::cdr::CDROutputStream& output) const
{
        CORBA::ULong length = value_length(m_digits);
        CORBA::Octet octet;
        for (CORBA::ULong i = 0; i < length; i++) {
          input.read_octet(octet);
                output.write_octet(octet);
        }
}




bool
TIDorb::core::typecode::FixedTypeCode::skip_value(TIDorb::core::cdr::CDRInputStream& input) const
{
  CORBA::ULong length = value_length(m_digits);
  input.skip_octet_array(length);
  return true;
}




bool TIDorb::core::typecode::FixedTypeCode::values_equal
        (TIDorb::core::cdr::CDRInputStream& a_input,
         TIDorb::core::cdr::CDRInputStream& b_input) const
{
   CORBA::Fixed fixed_a;
   CORBA::Fixed fixed_b;

   a_input.read_fixed(fixed_a, m_digits, m_scale) ;
   b_input.read_fixed(fixed_b, m_digits, m_scale);

   return fixed_a == fixed_b;

}




void TIDorb::core::typecode::FixedTypeCode::dump (ostream& output) const
{
  output << "[TYPECODE]{fixed<"
         << m_digits << ',' << m_scale <<">}";
}




bool TIDorb::core::typecode::FixedTypeCode::dump_value
        (TIDorb::core::cdr::CDRInputStream& input,
         ostream& output) const
{
  CORBA::Fixed fixed;

  input.read_fixed(fixed, m_digits, m_scale);

  output << "[VALUE] {fixed<"
         << m_digits << ',' << m_scale <<">}"
         << ": \"" << fixed << "\"}";

  return true;
}
