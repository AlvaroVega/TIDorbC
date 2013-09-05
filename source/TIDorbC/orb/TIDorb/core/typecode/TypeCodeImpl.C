/////////////////////////////////////////////////////////////////////////
//
// File:        TypeCodeImpl.C
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

TIDorb::core::typecode::TypeCodeImpl::TypeCodeImpl()
 throw (TIDThr::SystemException)
: m_kind(CORBA::tk_null), m_exhaustive_equal(false)
{
  _count(1);
}

TIDorb::core::typecode::TypeCodeImpl::TypeCodeImpl(CORBA::TCKind kind)
 throw (TIDThr::SystemException)
: m_kind(kind), m_exhaustive_equal(false)
{
  _count(1);
}

TIDorb::core::typecode::TypeCodeImpl::~TypeCodeImpl()
 throw (TIDThr::SystemException)
{}

void TIDorb::core::typecode::TypeCodeImpl::set_exhaustive_equal(bool value)
{
  m_exhaustive_equal = value;
}

CORBA::TCKind TIDorb::core::typecode::TypeCodeImpl::kind() const
{
  return m_kind;
}

CORBA::Boolean TIDorb::core::typecode::TypeCodeImpl::equal(CORBA::TypeCode_ptr tc) const
{
  if(!tc)
    throw CORBA::BAD_PARAM();
  return (m_kind == tc->kind());
}

CORBA::Boolean TIDorb::core::typecode::TypeCodeImpl::equivalent(CORBA::TypeCode_ptr tc) const
{
  if(!tc)
    throw CORBA::BAD_PARAM();

        if(tc->kind() == CORBA::tk_alias) {
                try { // the exception must never be thrown, but, it is in the definition.
                        return equivalent(tc->content_type());
                } catch (const CORBA::TypeCode::BadKind& ex) { return false;}
        } else if(kind() == CORBA::tk_alias) {
                try { // the exception must never be thrown, but, it is in the definition.
                        return content_type()->equivalent(tc);
                } catch (const CORBA::TypeCode::BadKind& ex) { return false;}
        } else
                return equal(tc);
}

CORBA::TypeCode_ptr TIDorb::core::typecode::TypeCodeImpl::get_compact_typecode() const
{
  return CORBA::TypeCode::_duplicate((CORBA::TypeCode_ptr) this);
}

const char* TIDorb::core::typecode::TypeCodeImpl::id() const
{
  throw CORBA::TypeCode::BadKind();
}

const char* TIDorb::core::typecode::TypeCodeImpl::name() const
{
  throw CORBA::TypeCode::BadKind();
}

CORBA::ULong TIDorb::core::typecode::TypeCodeImpl::member_count() const
{
  throw CORBA::TypeCode::BadKind();
}

const char* TIDorb::core::typecode::TypeCodeImpl::member_name(CORBA::ULong index) const
{
  throw CORBA::TypeCode::BadKind();
}

CORBA::TypeCode_ptr TIDorb::core::typecode::TypeCodeImpl::member_type(CORBA::ULong index) const
{
  throw CORBA::TypeCode::BadKind();
}

CORBA::Any* TIDorb::core::typecode::TypeCodeImpl::member_label(CORBA::ULong index) const
{
  throw CORBA::TypeCode::BadKind();
}

CORBA::TypeCode_ptr TIDorb::core::typecode::TypeCodeImpl::discriminator_type() const
{
  throw CORBA::TypeCode::BadKind();
}

CORBA::Long TIDorb::core::typecode::TypeCodeImpl::default_index() const
{
  throw CORBA::TypeCode::BadKind();
}

CORBA::ULong TIDorb::core::typecode::TypeCodeImpl::length() const
{
  throw CORBA::TypeCode::BadKind();
}

CORBA::TypeCode_ptr TIDorb::core::typecode::TypeCodeImpl::content_type() const
{
  throw CORBA::TypeCode::BadKind();
}

CORBA::UShort TIDorb::core::typecode::TypeCodeImpl::fixed_digits() const
{
  throw CORBA::TypeCode::BadKind();
}

CORBA::Short TIDorb::core::typecode::TypeCodeImpl::fixed_scale() const
{
  throw CORBA::TypeCode::BadKind();
}
#ifndef MINIMUN
CORBA::Visibility TIDorb::core::typecode::TypeCodeImpl::member_visibility(CORBA::ULong index) const
{
  throw CORBA::TypeCode::BadKind();
}
#endif

CORBA::ValueModifier TIDorb::core::typecode::TypeCodeImpl::type_modifier() const
{
  throw CORBA::TypeCode::BadKind();
}

CORBA::TypeCode_ptr TIDorb::core::typecode::TypeCodeImpl::concrete_base_type() const
{
  throw CORBA::TypeCode::BadKind();
}

/* inline
bool TIDorb::core::typecode::TypeCodeImpl::isSimple() const
{
  return true;
}
*/

/**
 * TypeCode Marshaling.
 */

void TIDorb::core::typecode::TypeCodeImpl::marshal(TIDorb::core::cdr::CDROutputStream& output)
{
        output.write_ulong(m_kind);
}

/**
 * TypeCode parameters unmarshalling. In this case, there is nothing to do.
 * This operation will throw the <code>INTERNAL</code> exception.
 */
void TIDorb::core::typecode::TypeCodeImpl::partial_unmarshal(TIDorb::core::cdr::CDRInputStream& input)
{
        throw  CORBA::INTERNAL(); // ("Nothing to unmarshal", 0, CompletionStatus.COMPLETED_NO);
}


  /**
   * Skips the value associated to the TypeCode. This operation is used by the
   * TIDorb's Any implementation an the subclass <code>skip_value()</code>
   * operations.
   * @param input must be always a reference to a CDRInputStream object.
   */

bool TIDorb::core::typecode::TypeCodeImpl::skip_value(TIDorb::core::cdr::CDRInputStream& input) const
{
    switch (m_kind) {
        case CORBA::tk_short:
                input.skip_short();
                break;
        case CORBA::tk_long:
                input.skip_long();
                break;
        case CORBA::tk_longlong:
                input.skip_longlong();
                break;
        case CORBA::tk_ushort:
                input.skip_ushort();
                break;
        case CORBA::tk_ulong:
                input.skip_ulong();
                break;
        case CORBA::tk_ulonglong:
                input.skip_ulonglong();
                break;
        case CORBA::tk_float:
                input.skip_float();
                break;
        case CORBA::tk_double:
                input.skip_double();
                break;
        case CORBA::tk_boolean:
                input.skip_boolean();
                break;
        case CORBA::tk_longdouble:
                input.skip_longdouble();
                break;
        case CORBA::tk_char:
                input.skip_char();
                break;
        case CORBA::tk_wchar:
                input.skip_wchar();
                break;
        case CORBA::tk_octet:
                input.skip_octet();
                break;
        case CORBA::tk_any:
                input.skip_any();
                break;
        case CORBA::tk_TypeCode:
                input.skip_TypeCode();
                break;
        case CORBA::tk_Principal:
                input.skip_Principal();
                break;
        case CORBA::tk_null:
        case CORBA::tk_void:
                break;
        default:
                throw CORBA::INTERNAL(); //("TypeCode is not basic.", 0, CompletionStatus.COMPLETED_NO);
    }
    return true;
}


/**
 * Copies and remarshals the given typecode value marshalled in an InputStream
 * to  a <code>TIDorb::core::CDRInputStream</code>.
 * This method will always be invoked by this stream.
 *
 * @param type the value <code>TypeCode</code>
 * @param input the <code>InputStream</code> where the value is marshalled
 * @param output the <code>TIDorb::core::CDRInputStream</code>
 * @pre the <code>TypeCode</code> must be a basic type
 */

void TIDorb::core::typecode::TypeCodeImpl::remarshal_value
      (TIDorb::core::cdr::CDRInputStream& input,
       TIDorb::core::cdr::CDROutputStream& output) const
{
   switch (m_kind) {
    case CORBA::tk_short:
    {
          CORBA::Short short_val;
          input.read_short(short_val);
          output.write_short(short_val);
          break;
    }
    case CORBA::tk_long:
    {
          CORBA::Long long_val;
          input.read_long(long_val);
          output.write_long(long_val);
          break;
    }
    case CORBA::tk_longlong:
    {
          CORBA::LongLong longlong_val;
          input.read_longlong(longlong_val);
          output.write_longlong(longlong_val);
          break;
    }
    case CORBA::tk_ushort:
    {
          CORBA::UShort ushort_val;
          input.read_ushort(ushort_val);
          output.write_ushort(ushort_val);
          break;
    }
    case CORBA::tk_ulong:
    {
          CORBA::ULong ulong_val;
          input.read_ulong(ulong_val);
          output.write_ulong(ulong_val);
          break;
    }
    case CORBA::tk_ulonglong:
    {
          CORBA::ULongLong ulonglong_val;
          input.read_ulonglong(ulonglong_val);
          output.write_ulonglong(ulonglong_val);
          break;
    }
    case CORBA::tk_float:
    {
          CORBA::Float float_val;
          input.read_float(float_val);
          output.write_float(float_val);
          break;
    }
    case CORBA::tk_double:
    {
          CORBA::Double double_val;
          input.read_double(double_val);
          output.write_double(double_val);
          break;
    }
    case CORBA::tk_boolean:
    {
          CORBA::Boolean boolean_val;
          input.read_boolean(boolean_val);
          output.write_boolean(boolean_val);
          break;
    }
    case CORBA::tk_longdouble:
    {
          CORBA::LongDouble longdouble_val;
          input.read_longdouble(longdouble_val);
          output.write_longdouble(longdouble_val);
          break;
    }
    case CORBA::tk_char:
    {
          CORBA::Char char_val;
          input.read_char(char_val);
          output.write_char(char_val);
          break;
    }
    case CORBA::tk_wchar:
    {
          CORBA::WChar wchar_val;
          input.read_wchar(wchar_val);
          output.write_wchar(wchar_val);
          break;
    }
    case CORBA::tk_octet:
    {
          CORBA::Octet octet_val;
          input.read_octet(octet_val);
          output.write_octet(octet_val);
          break;
    }
    case CORBA::tk_any:
    {
          CORBA::TypeCode_ptr tc;
          input.read_TypeCode(tc);

          TypeCodeImpl* tc_impl = (TypeCodeImpl*)tc;
          output.write_TypeCode(tc);
          tc_impl->remarshal_value(input,output);

          CORBA::release(tc);
          break;
    }
    case CORBA::tk_TypeCode:
    {
          CORBA::TypeCode_var tc;
          input.read_TypeCode(tc);
          output.write_TypeCode(tc);
          CORBA::release(tc);
    }
    case CORBA::tk_Principal:
    {
          char * princ;
          input.read_Principal(princ);
          output.write_Principal(princ);

          CORBA::string_free(princ);
          break;
    }
    case CORBA::tk_null:
    case CORBA::tk_void:
          return;
    default:
          throw CORBA::MARSHAL(); //("TypeCode is not basic",0, CompletionStatus.COMPLETED_NO);
  }
//EMLG
}



/**
 *  Compares two InputStream marshalled values of a given TypeCode
 *  to  a <code>TIDorb::core::CDRInputStream</code>.
 *  This method will always be invoked by this stream.
 *
 * @param type the value <code>TypeCode</code>
 * @param input_a the <code>InputStream</code> where one value is marshalled
 * @param input_b the <code>InputStream</code> where the value value is marshalled
 * @pre <code>type</code>must be a basic type.
 */

bool TIDorb::core::typecode::TypeCodeImpl::values_equal
      (TIDorb::core::cdr::CDRInputStream& a_input,
       TIDorb::core::cdr::CDRInputStream& b_input) const
{
    switch (m_kind) {
    case CORBA::tk_short:
    {
          CORBA::Short short_a, short_b;
          a_input.read_short(short_a);
          b_input.read_short(short_b);
          return short_a == short_b;
    }
    case CORBA::tk_long:
    {
          CORBA::Long long_a, long_b;
          a_input.read_long(long_a);
          b_input.read_long(long_b);
          return long_a == long_b;
    }
    case CORBA::tk_longlong:
    {
          CORBA::LongLong longlong_a, longlong_b;
          a_input.read_longlong(longlong_a);
          b_input.read_longlong(longlong_b);
          return longlong_a == longlong_b;
    }
    case CORBA::tk_ushort:
    {
          CORBA::UShort ushort_a, ushort_b;
          a_input.read_ushort(ushort_a);
          b_input.read_ushort(ushort_b);
          return ushort_a == ushort_b;
    }
    case CORBA::tk_ulong:
    {
          CORBA::ULong ulong_a, ulong_b;
          a_input.read_ulong(ulong_a);
          b_input.read_ulong(ulong_b);
          return ulong_a == ulong_b;
    }
    case CORBA::tk_ulonglong:
    {
          CORBA::ULongLong ulonglong_a, ulonglong_b;
          a_input.read_ulonglong(ulonglong_a);
          b_input.read_ulonglong(ulonglong_b);
          return ulonglong_a == ulonglong_b;
    }
    case CORBA::tk_float:
    {
          CORBA::Float float_a, float_b;
          a_input.read_float(float_a);
          b_input.read_float(float_b);
          return float_a == float_b;
    }
    case CORBA::tk_double:
    {
          CORBA::Double double_a, double_b;
          a_input.read_double(double_a);
          b_input.read_double(double_b);
          return double_a == double_b;
    }
    case CORBA::tk_boolean:
    {
          CORBA::Boolean boolean_a, boolean_b;
          a_input.read_boolean(boolean_a);
          b_input.read_boolean(boolean_b);
          return boolean_a == boolean_b;
    }
    case CORBA::tk_longdouble:
    {
          CORBA::LongDouble longdouble_a, longdouble_b;
          a_input.read_longdouble(longdouble_a);
          b_input.read_longdouble(longdouble_b);
          return longdouble_a == longdouble_b;
    }
    case CORBA::tk_char:
    {
          CORBA::Char char_a, char_b;
          a_input.read_char(char_a);
          b_input.read_char(char_b);
          return char_a == char_b;
    }
    case CORBA::tk_wchar:
    {
          CORBA::WChar wchar_a, wchar_b;
          a_input.read_wchar(wchar_a);
          b_input.read_wchar(wchar_b);
          return wchar_a == wchar_b;
    }
    case CORBA::tk_octet:
    {
          CORBA::Octet octet_a, octet_b;
          a_input.read_octet(octet_a);
          b_input.read_octet(octet_b);
          return octet_a == octet_b;
    }
    case CORBA::tk_any:
    {
          CORBA::TypeCode_var tc_a, tc_b;
          a_input.read_TypeCode(tc_a);
          b_input.read_TypeCode(tc_b);

          if (tc_a->equal(tc_b)) {
                TypeCodeImpl* tc_impl_a = (TypeCodeImpl*)((CORBA::TypeCode_ptr) tc_a);
                TypeCodeImpl* tc_impl_b = (TypeCodeImpl*)((CORBA::TypeCode_ptr) tc_b);
                return tc_impl_a->values_equal(a_input, b_input);
          } else {
                return false;
          }
    }
    case CORBA::tk_TypeCode:
    {
          CORBA::TypeCode_var tc_a, tc_b;
          a_input.read_TypeCode(tc_a);
          b_input.read_TypeCode(tc_b);

          return tc_a->equal(tc_b);
    }
    case CORBA::tk_Principal:
    {
          char* princ_a;
          char* princ_b;
          a_input.read_Principal(princ_a);
          b_input.read_Principal(princ_b);

          bool equal = !strcmp(princ_a, princ_b);

          CORBA::string_free(princ_a);
          CORBA::string_free(princ_b);

          return equal;
    }
    case CORBA::tk_null:
    case CORBA::tk_void:
          return true;
    default:
          throw CORBA::MARSHAL(); //("TypeCode is not basic",0, CompletionStatus.COMPLETED_NO);
  }
}

/**
 * Dumps the description of a given string TypeCode.
 * @param output the output stream where the TypeCode will be dumped
 * @pre <code>type</code>must be a string type.
 */

void TIDorb::core::typecode::TypeCodeImpl::dump (ostream& output) const
{
    output << "[TYPECODE]";
    switch (m_kind) {
        case CORBA::tk_short:
                output << "{short}";
                break;
        case CORBA::tk_long:
                output << "{long}";
                break;
        case CORBA::tk_longlong:
                output << "{longlong}";
                break;
        case CORBA::tk_ushort:
                output << "{ushort}";
                break;
        case CORBA::tk_ulong:
                output << "{ulong}";
                break;
        case CORBA::tk_ulonglong:
                output << "{ulonglong}";
                break;
        case CORBA::tk_float:
                output << "{float}";
                break;
        case CORBA::tk_double:
                output << "{double}";
                break;
        case CORBA::tk_boolean:
                output << "{boolean}";
                break;
        case CORBA::tk_longdouble:
                output << "{longdouble}";
                break;
        case CORBA::tk_char:
                output << "{char}";
                break;
        case CORBA::tk_wchar:
                output << "{wchar}";
                break;
        case CORBA::tk_octet:
                output << "{octet}";
                break;
        case CORBA::tk_any:
                output << "{any}";
                break;
        case CORBA::tk_string:
                output << "{string}";
                break;
        case CORBA::tk_wstring:
                output << "{wstring}";
                break;
        case CORBA::tk_TypeCode:
                output << "{TypeCode}";
                break;
        case CORBA::tk_Principal:
                output << "{Principal}";
        case CORBA::tk_null:
                output << "{null}";
                break;
        case CORBA::tk_void:
                output << "{void}";
                break;
        default:
                break;
   }
}

/**
 * Dumps the description of a the marshalled value of a given TypeCode.
 * @param input the input stream where the value is marshaled
 * @param output the output stream where the value will be dumped
 * @pre the typecode must be a simple type
 */


bool TIDorb::core::typecode::TypeCodeImpl::dump_value(TIDorb::core::cdr::CDRInputStream& input,
                                                      ostream& output) const
{
        switch (m_kind) {
        case CORBA::tk_null:
        {
                output << "{null value}";
                return true;
        }
        case CORBA::tk_short:
        {
                CORBA::Short short_val;
                input.read_short(short_val);
                output << "{short: " << short_val << "}";
                return true;
        }
        case CORBA::tk_long:
        {
                CORBA::Long long_val;
                input.read_long(long_val);
                output << "{long: " << long_val << "}";
                return true;
        }
        case CORBA::tk_longlong:
        {
                CORBA::LongLong longlong_val;
                input.read_longlong(longlong_val);
                output << "{longlong: " << longlong_val << "}";
                return true;
        }
        case CORBA::tk_ushort:
        {
                CORBA::UShort ushort_val;
                input.read_ushort(ushort_val);
                output << "{ushort: " << ushort_val << "}";
                return true;
        }
        case CORBA::tk_ulong:
        {
                CORBA::ULong ulong_val;
                input.read_ulong(ulong_val);
                output << "{ulong: " << ulong_val << "}";
                return true;
        }
        case CORBA::tk_ulonglong:
        {
                CORBA::ULongLong ulonglong_val;
                input.read_ulonglong(ulonglong_val);
                output << "{ulonglong: " << ulonglong_val << "}";
                return true;
        }
        case CORBA::tk_float:
        {
                CORBA::Float float_val;
                input.read_float(float_val);
                output << "{float: " << float_val << "}";
                return true;
        }
        case CORBA::tk_double:
        {
                CORBA::Double double_val;
                input.read_double(double_val);
                output << "{double: " << double_val << "}";
                return true;
        }
        case CORBA::tk_boolean:
        {
                CORBA::Boolean boolean_val;
                input.read_boolean(boolean_val);
                output << "{boolean: " << boolean_val << "}";
                return true;
        }
        case CORBA::tk_longdouble:
        {
                CORBA::LongDouble longdouble_val;
                input.read_longdouble(longdouble_val);
                output << "{longdouble: " << longdouble_val << "}";
                return true;
        }
        case CORBA::tk_char:
        {
                CORBA::Char char_val;
                input.read_char(char_val);
                output << "{char: " << char_val << "}";
                return true;
        }
        case CORBA::tk_wchar:
        {
                CORBA::WChar wchar_val;
                input.read_wchar(wchar_val);
                output << "{wchar: " << wchar_val << "}";
                return true;
        }
        case CORBA::tk_octet:
        {
                CORBA::Octet octet_val;
                input.read_octet(octet_val);
                output << "{octet: " << octet_val << "}";
                return true;
        }
        case CORBA::tk_any:
        {
                output << "{any: ";
                CORBA::TypeCode_ptr tc = 0;

                input.read_TypeCode(tc);

                TypeCodeImpl* tc_impl = (TypeCodeImpl*)tc;

                tc_impl->dump(output);
                tc_impl->dump_value(input,output);
                output << " }";
                CORBA::release(tc);
                return true;
        }
        case CORBA::tk_TypeCode:
        {
                CORBA::TypeCode_ptr tc = 0;
                input.read_TypeCode(tc);

                TypeCodeImpl* tc_impl = (TypeCodeImpl*)tc;

                output << "{typecode: ";
                tc_impl->dump(output);
                output << " }";

                CORBA::release(tc);
                return true;
        }
        case CORBA::tk_Principal:
        {
                char* princ;
                input.read_Principal(princ);
                output << "{principal: " << princ << " }";
                CORBA::string_free(princ);
                return true;
        }
        default:
          break;
         
  }

  return false;
}



CORBA::TypeCode_ptr CORBA::TypeCode::_duplicate(CORBA::TypeCode_ptr tc)
{
  try {

    TIDorb::core::typecode::TypeCodeImpl* tc_impl = NULL;
    if (tc)
	tc_impl = (TIDorb::core::typecode::TypeCodeImpl*)tc;

    if(tc_impl){
      tc_impl->_add_ref();
      //      cerr <<" FINAL CORBA::TypeCode::_duplicate _count " << tc_impl->_count() << " " << tc_impl<< endl;
    }
    return tc;

  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL();
  }
}


CORBA::TypeCode_ptr CORBA::TypeCode::_nil()
{
  return 0;
}

void CORBA::release(CORBA::TypeCode_ptr tc)
{
  //  cerr << "INICIO CORBA::_release tc " << tc << " ";
  try {
    TIDorb::core::typecode::TypeCodeImpl* tc_impl = NULL;
    //if (tc)
    	tc_impl = (TIDorb::core::typecode::TypeCodeImpl*)tc;
    
    if(tc_impl){
      //      cerr << "FINAL CORBA::_release _count " << tc_impl->_count() << " " << tc_impl << endl;
      tc_impl->_remove_ref();
    }
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL();
  }
}

CORBA::Boolean CORBA::is_nil(CORBA::TypeCode_ptr tc)
{
  return !tc;
}


::CORBA::TypeCode_ptr CORBA::_TypeCodeHelper::type()
{
  return new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_TypeCode);
}

const char* CORBA::_TypeCodeHelper::id()
{
  return "IDL:CORBA/TypeCode:1.0";
}

void CORBA::_TypeCodeHelper::insert(::CORBA::Any& any, CORBA::TypeCode_ptr _value)
{
  any <<= _value;
}

void CORBA::_TypeCodeHelper::extract(const ::CORBA::Any& any, CORBA::TypeCode_ptr& _value)
{
  any >>= _value;
}

void CORBA::_TypeCodeHelper::read(::TIDorb::portable::InputStream& is, CORBA::TypeCode_ptr& val)
{
  CORBA::release(val);
  is.read_TypeCode(val);
}

void CORBA::_TypeCodeHelper::write(::TIDorb::portable::OutputStream& os, const CORBA::TypeCode_ptr val)
{
  os.write_TypeCode(val);
}

CORBA::_TypeCodeHolder::_TypeCodeHolder()
{
  value = 0;
}

CORBA::_TypeCodeHolder::_TypeCodeHolder(const CORBA::TypeCode_ptr initial)
{
  value=CORBA::TypeCode::_duplicate(initial);
}
CORBA::TypeCode_ptr CORBA::_TypeCodeHolder::_type() const
{
  return TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(CORBA::tk_TypeCode);
}

void CORBA::_TypeCodeHolder::_write(::TIDorb::portable::OutputStream& outs) const
{
  CORBA::_TypeCodeHelper::write(outs,value);
}

void CORBA::_TypeCodeHolder::_read(::TIDorb::portable::InputStream& ins)
{
  CORBA::release(value);
  CORBA::_TypeCodeHelper::read(ins,value);
}
