/////////////////////////////////////////////////////////////////////////
//
// File:        AnyImpl.C
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

#include "TIDorb/core.h"
#include "TIDorb/core/util.h"

#include "TIDorb/core/util/GenericValue.h"

#include <wchar.h>


CORBA::Any::Any()
{
  m_delegate = new TIDorb::core::AnyImpl();
}

CORBA::Any::~Any()
{
  delete m_delegate;
}

CORBA::Any::Any(const CORBA::Any& a)
{
  m_delegate = new TIDorb::core::AnyImpl();
  (*m_delegate) = a.delegate();
}

CORBA::Any& CORBA::Any::operator=(const CORBA::Any& value)
{
  (*m_delegate) = value.delegate();
  return *this;
}

bool CORBA::Any::operator==(const CORBA::Any& value) const
{
  return (*m_delegate) == value.delegate();
}

bool CORBA::Any::operator!=(const CORBA::Any& value) const
{
  return (*m_delegate) == value.delegate();
}


CORBA::TypeCode_ptr TIDorb::core::AnyImpl::get_effective_type(const CORBA::TypeCode_ptr tc)
{
  if (tc->kind() == CORBA::tk_alias) {
    return tc->content_type();
  } else {
    return CORBA::TypeCode::_duplicate(tc);
  }
}


TIDorb::core::AnyImpl::AnyImpl()
{
  mi_cdr=TIDorb::core::AnyImpl::OK;
  my_any_ref=NULL;
  my_typecode=NULL;

  m_orb = NULL;

  m_type = (TIDorb::core::typecode::TypeCodeImpl*)TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(CORBA::tk_null);
  m_effective_type = NULL;
  m_marshaled_value = NULL;
  m_value = NULL;
  m_wrapped_any = NULL;
}

TIDorb::core::AnyImpl::~AnyImpl()
{
  CORBA::release(m_type);
  CORBA::release(m_effective_type);

 // reset_value();
 if(m_value)
    m_value->_remove_ref();
 delete m_marshaled_value;

}

TIDorb::core::TIDORB* TIDorb::core::AnyImpl::orb() const
{
  return m_orb;
}

void TIDorb::core::AnyImpl::orb(TIDorb::core::TIDORB* ptr)
{
  m_orb = ptr;
  if (m_orb)
    m_type->set_exhaustive_equal(m_orb->conf().exhaustive_equal);
}

CORBA::TypeCode_ptr TIDorb::core::AnyImpl::type() const
{
  return CORBA::TypeCode::_duplicate(m_type);
}

void TIDorb::core::AnyImpl::type(CORBA::TypeCode_ptr tc)
{
  if(!m_type->equivalent(tc))
    throw CORBA::BAD_TYPECODE();

  CORBA::release(m_effective_type);
  m_effective_type = get_effective_type(tc);

  CORBA::release(m_type);
  m_type = (TIDorb::core::typecode::TypeCodeImpl*)CORBA::TypeCode::_duplicate(tc);
}

void TIDorb::core::AnyImpl::set_equivalent_type(CORBA::TypeCode_ptr tc)
{
  CORBA::release(m_effective_type);
  m_effective_type = get_effective_type(tc);

  CORBA::release(m_type);
  m_type = (TIDorb::core::typecode::TypeCodeImpl*)CORBA::TypeCode::_duplicate(tc);

}

void TIDorb::core::AnyImpl::set_type(CORBA::TypeCode_ptr tc) 
{
  CORBA::release(m_effective_type);
  m_effective_type = get_effective_type(tc);

  CORBA::release(m_type);
  m_type = (TIDorb::core::typecode::TypeCodeImpl*)CORBA::TypeCode::_duplicate(tc);

  reset_value();
}

void TIDorb::core::AnyImpl::reset_value()
{
  if(m_value){
    m_value->_remove_ref();
    m_value = NULL;
  }
  //tenemos que guardar el buffer para que el siguiente tenga el puntero correcto
  //solo si estamos en skip mode optimizado
  //if(m_marshaled_value){
  if(m_marshaled_value&&my_any_ref==NULL){
    delete m_marshaled_value;
    m_marshaled_value = NULL;
  }
  m_wrapped_any = NULL;
}

bool TIDorb::core::AnyImpl::has_Streamable() const
{
  return m_value != NULL;
}

// asigns the new vaule and reads from the _marshaled value.
void TIDorb::core::AnyImpl::init_value(TIDorb::portable::Streamable* holder)
{
  //skip optimization
  if (mi_cdr==TIDorb::core::AnyImpl::DEPENDIENTE)
  {
    my_any_ref->skip_value(m_marshaled_value);
  }
  if (m_marshaled_value) {
    m_marshaled_value->rewind();
    m_value = holder;
    m_value->_read(*m_marshaled_value);
    //skip optimization
    mi_cdr=TIDorb::core::AnyImpl::OK;
  } else {
    throw CORBA::BAD_OPERATION();//("Any without value.", 0, CompletionStatus.COMPLETED_NO);
  }
}


bool TIDorb::core::AnyImpl::operator!=(const TIDorb::portable::Any& a) const
{
  return ! (*this == a);
}

bool TIDorb::core::AnyImpl::operator==(const TIDorb::portable::Any& a) const
{
  if(m_wrapped_any)
    return (a == *m_wrapped_any);

  CORBA::TypeCode_var other_type = a.type();

  if (!m_type->equivalent(other_type))
    return false;

  switch (m_type->kind()) {
    case CORBA::tk_null:
    case CORBA::tk_void:
    {
      return true;
    }
    case CORBA::tk_short:
    {
      CORBA::Short my_value, value;
      a.extract_short(value);
      extract_short(my_value);
      return my_value == value;
    }
    case CORBA::tk_long:
    {
      CORBA::Long my_value, value;
      a.extract_long(value);
      extract_long(my_value);
      return my_value == value;
    }
    case CORBA::tk_longlong:
    {
      CORBA::LongLong my_value, value;
      a.extract_longlong(value);
      extract_longlong(my_value);
      return my_value == value;
    }
    case CORBA::tk_ushort:
    {
      CORBA::UShort my_value, value;
      a.extract_ushort(value);
      extract_ushort(my_value);
      return my_value == value;
    }
    case CORBA::tk_ulong:
    {
      CORBA::ULong my_value, value;
      a.extract_ulong(value);
      extract_ulong(my_value);
      return my_value == value;
    }
    case CORBA::tk_ulonglong:
    {
      CORBA::ULongLong my_value, value;
      a.extract_ulonglong(value);
      extract_ulonglong(my_value);
      return my_value == value;
    }
    case CORBA::tk_float:
    {
      CORBA::Float my_value, value;
      a.extract_float(value);
      extract_float(my_value);
      return my_value == value;
    }
    case CORBA::tk_double:
    {
      CORBA::Double my_value, value;
      a.extract_double(value);
      extract_double(my_value);
      return my_value == value;
    }
    case CORBA::tk_boolean:
    {
      CORBA::Boolean my_value, value;
      a.extract_boolean(value);
      extract_boolean(my_value);
      return my_value == value;
    }
    case CORBA::tk_char:
     {
      CORBA::Char my_value, value;
      a.extract_char(value);
      extract_char(my_value);
      return my_value == value;
    }
    case CORBA::tk_wchar:
    {
      CORBA::WChar my_value, value;
      a.extract_wchar(value);
      extract_wchar(my_value);
      return my_value == value;
    }
    case CORBA::tk_octet:
    {
      CORBA::Octet my_value, value;
      a.extract_octet(value);
      extract_octet(my_value);
      return my_value == value;
    }
    case CORBA::tk_longdouble:
    {
      CORBA::LongDouble my_value, value;
      a.extract_longdouble(value);
      extract_longdouble(my_value);
      return my_value == value;
    }
    case CORBA::tk_any:
    {
      const CORBA::Any* my_value;
      const CORBA::Any* value;
      a.extract_any(value);
      extract_any(my_value);
      //TODO: try to remove dynamic_cast
      bool equal = (dynamic_cast<AnyImpl& >(my_value->delegate()) ==
                    dynamic_cast<AnyImpl& >(value->delegate()));
      return equal;
    }
    case CORBA::tk_objref:
    {
      CORBA::Object_var my_value, value;
      a.extract_Object(value);
      extract_Object(my_value);
 
      if(!my_value) {
        return (bool)(!value);
      } else {
        return my_value->_is_equivalent(value);
      }
    }
    case CORBA::tk_string:
    {
      try
      {
        if(m_type->length() == 0) {
          const char* my_value;
          const char* value;

          extract_string(my_value);
          a.extract_string(value);
          return !strcmp(my_value, value);
        }
      } catch (const CORBA::TypeCode::BadKind& bk) {
        throw CORBA::BAD_TYPECODE();
      }
      // bounded string
      break;
    }
    case CORBA::tk_wstring:
    {
      try
      {
        if(m_type->length() == 0) {
          const CORBA::WChar* my_value;
          const CORBA::WChar* value;
          a.extract_wstring(value);
          extract_wstring(my_value);
          return !wcscmp(my_value, value);
        }
      } catch (const CORBA::TypeCode::BadKind& bk) {
        throw CORBA::BAD_TYPECODE();
      }
      // bounded wstring
      break;
    }
    case CORBA::tk_TypeCode:
    {
      CORBA::TypeCode_var my_value, value;
      a.extract_TypeCode(value);
      extract_TypeCode(my_value);
      return my_value->equal(value);
    }
    case CORBA::tk_fixed:
    {
      CORBA::Fixed my_value, value;
      a.extract_fixed(value, (CORBA::TypeCode_ptr) m_type);
      extract_fixed(my_value, (CORBA::TypeCode_ptr) m_type);
      return my_value == value;
    }
    default:
      break;
  }
 
  TIDorb::core::cdr::CDRInputStream* input =
    (TIDorb::core::cdr::CDRInputStream*) (create_input_stream());
  TIDorb::core::cdr::CDRInputStream* input_a =
    (TIDorb::core::cdr::CDRInputStream*) (a.create_input_stream());
  
  bool equals = m_type->values_equal(*input, *input_a);

  delete input;
  delete input_a;

  return equals;
}

void TIDorb::core::AnyImpl::wrap_value(const TIDorb::core::AnyImpl* a)
{
  CORBA::TypeCode_var other_type = a->type();

  set_type(other_type);

  m_wrapped_any = a;
}
//returned TIDorb::core::AnyImpl&, changed due to covariant type return
// faults in gcc 2.95
TIDorb::portable::Any& TIDorb::core::AnyImpl::operator=(const TIDorb::portable::Any& a)
{
  //TODO revis 
  const AnyImpl& ref = * ( const AnyImpl* ) (&a);
  assign(ref);
  return *this;
}

void TIDorb::core::AnyImpl::assign(const TIDorb::core::AnyImpl& a, bool wrap)
{
  CORBA::TypeCode_var a_type = a.type();
  CORBA::TypeCode_var a_effective_type = get_effective_type(a_type);

  switch (a_effective_type->kind())
  {
    case CORBA::tk_null:
    case CORBA::tk_void:
    {
      set_type(a_type);
      return;
    }
    case CORBA::tk_short:
    {
      CORBA::Short value;
      a.extract_short(value);
      insert_short(value, a_type);
      return;
    }
    case CORBA::tk_long:
    {
      CORBA::Long value;
      a.extract_long(value);
      insert_long(value, a_type);
      return;
    }
    case CORBA::tk_longlong:
    {
      CORBA::LongLong value;
      a.extract_longlong(value);
      insert_longlong(value, a_type);
      return;
    }
    case CORBA::tk_ushort:
    {
      CORBA::UShort value;
      a.extract_ushort(value);
      insert_ushort(value, a_type);
      return;
    }
    case CORBA::tk_ulong:
    {
      CORBA::ULong value;
      a.extract_ulong(value);
      insert_ulong(value, a_type);
      return;
    }
    case CORBA::tk_ulonglong:
    {
      CORBA::ULongLong value;
      a.extract_ulonglong(value);
      insert_ulonglong(value, a_type);
      return;
    }
    case CORBA::tk_float:
    {
      CORBA::Float value;
      a.extract_float(value);
      insert_float(value, a_type);
      return;
    }
    case CORBA::tk_double:
    {
      CORBA::Double value;
      a.extract_double(value);
      insert_double(value, a_type);
      return;
    }
    case CORBA::tk_boolean:
    {
      CORBA::Boolean value;
      a.extract_boolean(value);
      insert_boolean(value, a_type);
      return;
    }
    case CORBA::tk_char:
    {
      CORBA::Char value;
      a.extract_char(value);
      insert_char(value, a_type);
      return;
    }
    case CORBA::tk_wchar:
    {
      CORBA::WChar value;
      a.extract_wchar(value);
      insert_wchar(value, a_type);
      return;
    }
    case CORBA::tk_octet:
    {
      CORBA::Octet value;
      a.extract_octet(value);
      insert_octet(value, a_type);
      return;
    }
    case CORBA::tk_longdouble:
    {
      CORBA::LongDouble value;
      a.extract_longdouble(value);
      insert_longdouble(value, a_type);
      return;
    }
    case CORBA::tk_any:
    {
      const CORBA::Any* value;
      a.extract_any(value);
      insert_any(*value);
      return;
    }
    case CORBA::tk_objref:
    {
      CORBA::Object_var value;
      a.extract_Object(value);
      insert_Object(value);
      return;
    }
    case CORBA::tk_string:
    {
      try {
        if(a_type->length() == 0) {
          const char* value;
          a.extract_string(value);
          insert_string(value);
          return;
        }
      } catch (const CORBA::TypeCode::BadKind& bk) {
        throw CORBA::BAD_TYPECODE();
      }
      // bounded string
      break;
    }
    case CORBA::tk_wstring:
    {
      try {
        if(a_type->length() == 0) {
          const CORBA::WChar* value;
          a.extract_wstring(value);
          insert_wstring(value);
        }
      } catch (const CORBA::TypeCode::BadKind& bk) {
        throw CORBA::BAD_TYPECODE();
      }
       // bounded wstring
      break;
    }
    case CORBA::tk_TypeCode:
    {
      CORBA::TypeCode_var value;
      a.extract_TypeCode(value);
      insert_TypeCode(value);
      return;
    }
    case CORBA::tk_fixed:
    {
      CORBA::Fixed value;
      a.extract_fixed(value, (CORBA::TypeCode_ptr) m_type);
      insert_fixed(value, (CORBA::TypeCode_ptr) m_type);
      return;
    }
    default:
      break;
  }


  if(a.has_Streamable()) {
    const TIDorb::portable::Streamable* streamable;
    a.extract_Streamable(streamable);
    insert_Streamable(streamable);
    return;
  }


  // Only wraps complex types

  if(wrap) {
    wrap_value(&a);
    return;
  }

  TIDorb::portable::InputStream* input = a.create_input_stream();
  // se desactiva el algoritmo de aplanado rapido
  // read_value(*input, a_type); 
  read_value(*input, a_type, true); 
  delete input;
}

void TIDorb::core::AnyImpl::assign_value(const TIDorb::core::AnyImpl& a, bool wrap)
{
  CORBA::TypeCode_var a_type = a.type();

  if(!m_type->equivalent(a_type))
    throw CORBA::BAD_OPERATION();//  ("No Equivalent types.", 0, CompletionStatus.COMPLETED_NO);

  assign(a, wrap);
}




void TIDorb::core::AnyImpl::read_value(TIDorb::portable::InputStream& input,
                                       CORBA::TypeCode_ptr tc, bool skip_mode)
{
  // optimiza el skip del buffer
  TIDorb::core::cdr::CDRInputStream& is1 = *(TIDorb::core::cdr::CDRInputStream*)(&input);

  if (!skip_mode)
  {
    if (!is1.point_ok)
    {
      // hay que borrar el anterior 
      reset_value(); 
      my_any_ref= is1.get_any_ref();
      my_typecode= (TIDorb::core::typecode::TypeCodeImpl*)tc;
      is1.put_any_ref(this);
      mi_cdr=TIDorb::core::AnyImpl::DEPENDIENTE;
      return;
    }
  }

  TIDorb::core::AnyImpl::Estado temp_cdr=mi_cdr;
  mi_cdr=TIDorb::core::AnyImpl::OK; 
  CORBA::TypeCode_var effective_tc = get_effective_type(tc);
  switch (effective_tc->kind())
  {
    case CORBA::tk_null:
    case CORBA::tk_void:
      set_type(tc);
      return;
    case CORBA::tk_short:
    {
      CORBA::Short value;
      input.read_short(value);
      insert_short(value, tc);
      return;
    }
    case CORBA::tk_long:
    {
      CORBA::Long value;
      input.read_long(value);
      insert_long(value, tc);
      return;
    }
    case CORBA::tk_longlong:
    {
      CORBA::LongLong value;
      input.read_longlong(value);
      insert_longlong(value, tc);
      return;
    }
    case CORBA::tk_ushort:
    {
      CORBA::UShort value;
      input.read_ushort(value);
      insert_ushort(value, tc);
      return;
    }
    case CORBA::tk_ulong:
    {
      CORBA::ULong value;
      input.read_ulong(value);
      insert_ulong(value, tc);
      return;
    }
    case CORBA::tk_ulonglong:
    {
      CORBA::ULongLong value;
      input.read_ulonglong(value);
      insert_ulonglong(value, tc);
      return;
    }
    case CORBA::tk_float:
    {
      CORBA::Float value;
      input.read_float(value);
      insert_float(value, tc);
      return;
    }
    case CORBA::tk_double:
    {
      CORBA::Double value;
      input.read_double(value);
      insert_double(value, tc);
      return;
    }
    case CORBA::tk_boolean:
    {
      CORBA::Boolean value;
      input.read_boolean(value);
      insert_boolean(value, tc);
      return;
    }
    case CORBA::tk_char:
     {
      CORBA::Char value;
      input.read_char(value);
      insert_char(value, tc);
      return;
    }
    case CORBA::tk_wchar:
     {
      CORBA::WChar value;
      input.read_wchar(value);
      insert_wchar(value, tc);
      return;
    }
    case CORBA::tk_octet:
    {
      CORBA::Octet value;
      input.read_octet(value);
      insert_octet(value, tc);
      return;
    }
    case CORBA::tk_longdouble:
    {
      CORBA::LongDouble value;
      input.read_longdouble(value);
      insert_longdouble(value, tc);
      return;
    }
    case CORBA::tk_any:
    {
      CORBA::Any* value = new CORBA::Any;
      input.read_any(*value);
      insert_any(value);
      return;
    }
    case CORBA::tk_objref:
    {
      CORBA::Object_var value;
      input.read_Object(value);
      insert_Object(value);
      return;
    }
    case CORBA::tk_string:
    {
      try {
        if(effective_tc->length() == 0) {
          char* value;
          input.read_string(value);
          insert_string(value);
          //TODO: posible memory leak?
          //CORBA::string_free(value);
          return;
        }
        else{
          // bounded string
          char* value;
          input.read_string(value, effective_tc->length()); // -> read_bounded_string
          insert_string(value);
          return;
        }
      } catch (const CORBA::TypeCode::BadKind& bk) {
        throw CORBA::BAD_TYPECODE();
      }

      break;
    }
    case CORBA::tk_wstring:
    {
      try {
        if(effective_tc->length() == 0) {
          CORBA::WChar* value;
          input.read_wstring(value);
          insert_wstring(value);
          //TODO: posible memory leak?
          //CORBA::wstring_free(value);
          return;
        }
      } catch (const CORBA::TypeCode::BadKind& bk) {
        throw CORBA::BAD_TYPECODE();
      }
      // bounded wstring
      break;
    }
    case CORBA::tk_TypeCode:
    {
      CORBA::TypeCode_var value;
      input.read_TypeCode(value);
      insert_TypeCode(value);
      return;
    }
    case CORBA::tk_fixed:
    {
      CORBA::Fixed value;
      input.read_fixed(value, effective_tc->fixed_digits(), effective_tc->fixed_scale());
      insert_fixed(value, effective_tc);
      return;
    }
    default:
      break;
  }
  mi_cdr=temp_cdr; 
  TIDorb::core::cdr::CDRInputStream& is = *(TIDorb::core::cdr::CDRInputStream*)(&input);
  if (!skip_mode)
  { 
    mi_cdr=TIDorb::core::AnyImpl::PRIMERO;
    set_type(tc);

  
    m_marshaled_value = is.copy();
  
    // saves the actual position
  
    m_marshaled_value->fix_starting();
  
    // set the input stream to the end of the value
  
    // optimiza el skip del buffer  
    is.point_ok=false;
    is.put_any_ref(this); 
  } 
  else
  { 
    set_type(tc); 
    m_marshaled_value = is.copy(); 
    m_marshaled_value->fix_starting(); 
    m_type->skip_value(is);
  }
}


void TIDorb::core::AnyImpl::skip_value(TIDorb::core::cdr::CDRInputStream* & input)
{
  if (mi_cdr==TIDorb::core::AnyImpl::OK)
  {
    input=m_marshaled_value->copy();
  }
  else if (mi_cdr==TIDorb::core::AnyImpl::PRIMERO) 
  { 
    input=m_marshaled_value->copy();
    read_value(*input,m_type,true);
  }
  else
  {
    my_any_ref->skip_value(input); 
    if(m_marshaled_value!=input)
      m_marshaled_value=((TIDorb::core::cdr::CDRInputStream*)input)->copy();
    if (my_typecode) // Take care, typecode is not duplicated
      read_value(*input,my_typecode,true);
    else   
      read_value(*input,m_type,true);
    
    mi_cdr=TIDorb::core::AnyImpl::PRIMERO;
  } 
  input->fix_starting(); 
}


void TIDorb::core::AnyImpl::write_value(TIDorb::portable::OutputStream& os) const
{
  if(m_wrapped_any) {
    m_wrapped_any->write_value(os);
    return;
  }

  CORBA::TCKind kind = m_type->kind();

  if( (kind == CORBA::tk_null) || (kind == CORBA::tk_void)) {
    return;
  } else if (m_value) {
    m_value->_write(os);
  } else if (m_marshaled_value){
    TIDorb::core::cdr::CDRInputStream* _marshaled_value_copy = m_marshaled_value->copy();

    TIDorb::core::cdr::CDROutputStream& output =
      *(TIDorb::core::cdr::CDROutputStream*) (&os);

    _marshaled_value_copy->rewind();

    m_type->remarshal_value(*_marshaled_value_copy, output);

    delete _marshaled_value_copy;

  } else {
    throw CORBA::BAD_OPERATION(); // ("Uncompleted Any.", 0, CompletionStatus.COMPLETED_NO);
  }
}


TIDorb::portable::OutputStream* TIDorb::core::AnyImpl::create_output_stream()
{
  reset_value();

  TIDorb::core::cdr::CDROutputStream* out = new TIDorb::core::cdr::CDROutputStream(m_orb);

  m_marshaled_value =
    (TIDorb::core::cdr::CDRInputStream*) (out->create_input_stream());

  return out;
}


TIDorb::portable::InputStream* TIDorb::core::AnyImpl::create_input_stream() const
{
  if(m_wrapped_any)
    return m_wrapped_any->create_input_stream();

  if(!m_marshaled_value)
    create_marshaled_value();

  TIDorb::core::cdr::CDRInputStream* new_stream = m_marshaled_value->copy();
  new_stream->rewind();
  return new_stream;
}

void TIDorb::core::AnyImpl::create_marshaled_value() const
{
  if(m_marshaled_value)
    return;

  CORBA::TCKind kind = m_type->kind();

  if((kind == CORBA::tk_null) || (kind == CORBA::tk_void)) {
    //m_orb->create_output_stream();
    ((TIDorb::core::AnyImpl*) this)->m_marshaled_value =
      new TIDorb::core::cdr::CDRInputStream(m_orb, new TIDorb::core::cdr::BufferCDR((CORBA::ULong) 0));
    return;
  }

  if (m_value) {
    TIDorb::core::cdr::CDROutputStream output(m_orb);

    m_value->_write(output);

    ((TIDorb::core::AnyImpl*) this)->m_marshaled_value =
      (TIDorb::core::cdr::CDRInputStream*)(output.create_input_stream());

  } else {
    throw  CORBA::BAD_OPERATION(); // ("Any without value.", 0, CompletionStatus.COMPLETED_NO);
  }
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_octet(CORBA::Octet& value) const
{
  if (m_effective_type->kind() != CORBA::tk_octet)
    return false;

  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 

  TIDorb::core::util::OctetHolder* holder =
    (TIDorb::core::util::OctetHolder*) (m_value);
  
  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_octet(CORBA::Octet s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::OctetHolder* holder = new TIDorb::core::util::OctetHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;
  
  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_boolean(CORBA::Boolean& value) const
{
  if (m_effective_type->kind() != CORBA::tk_boolean)
    return false;

  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::BooleanHolder* holder =
    (TIDorb::core::util::BooleanHolder*) (m_value);

  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_boolean(CORBA::Boolean s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::BooleanHolder* holder = new TIDorb::core::util::BooleanHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();


  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_char(CORBA::Char& value) const
{
  if (m_effective_type->kind() != CORBA::tk_char)
    return false;

  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::CharHolder* holder =
    (TIDorb::core::util::CharHolder*) (m_value);

  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_char(CORBA::Char s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::CharHolder* holder = new TIDorb::core::util::CharHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_wchar(CORBA::WChar& value) const
{
  if (m_effective_type->kind() != CORBA::tk_wchar)
    return false;

  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::WCharHolder* holder =
    (TIDorb::core::util::WCharHolder*) (m_value);
  
  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_wchar(CORBA::WChar s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::WCharHolder* holder = new TIDorb::core::util::WCharHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();


  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}


CORBA::Boolean TIDorb::core::AnyImpl::extract_short(CORBA::Short& value) const
{
  if (m_effective_type->kind() != CORBA::tk_short)
    return false;

  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::ShortHolder* holder =
    (TIDorb::core::util::ShortHolder*) (m_value);
  
  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_short(CORBA::Short s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::ShortHolder* holder = new TIDorb::core::util::ShortHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_ushort(CORBA::UShort& value) const
{
  if (m_effective_type->kind() != CORBA::tk_ushort)
    return false;
  
  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::UShortHolder* holder =
    (TIDorb::core::util::UShortHolder*) (m_value);
  
  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_ushort(CORBA::UShort s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::UShortHolder* holder = new TIDorb::core::util::UShortHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_long(CORBA::Long& value) const
{
  if (m_effective_type->kind() != CORBA::tk_long)
    return false;

  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::LongHolder* holder =
  (TIDorb::core::util::LongHolder*) (m_value);

  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_long(CORBA::Long s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::LongHolder* holder = new TIDorb::core::util::LongHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_ulong(CORBA::ULong& value) const
{
  if (m_effective_type->kind() != CORBA::tk_ulong)
    return false;

  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::ULongHolder* holder =
  (TIDorb::core::util::ULongHolder*) (m_value);

  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_ulong(CORBA::ULong s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::ULongHolder* holder = new TIDorb::core::util::ULongHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_longlong(CORBA::LongLong& value) const
{
  if (m_effective_type->kind() != CORBA::tk_longlong)
    return false;
  
  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::LongLongHolder* holder =
    (TIDorb::core::util::LongLongHolder*) (m_value);

  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_longlong(CORBA::LongLong s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::LongLongHolder* holder = new TIDorb::core::util::LongLongHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_ulonglong(CORBA::ULongLong& value) const
{
  if (m_effective_type->kind() != CORBA::tk_ulonglong)
    return false;

  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::ULongLongHolder* holder =
    (TIDorb::core::util::ULongLongHolder*) (m_value);

  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_ulonglong(CORBA::ULongLong s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::ULongLongHolder* holder = new TIDorb::core::util::ULongLongHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_float(CORBA::Float& value) const
{
  if (m_effective_type->kind() != CORBA::tk_float)
    return false;
  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::FloatHolder* holder =
    (TIDorb::core::util::FloatHolder*) (m_value);

  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_float(CORBA::Float s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::FloatHolder* holder = new TIDorb::core::util::FloatHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_double(CORBA::Double& value) const
{
  if (m_effective_type->kind() != CORBA::tk_double)
    return false;
  // skip optimization 
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::DoubleHolder* holder =
    (TIDorb::core::util::DoubleHolder*) (m_value);

  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_double(CORBA::Double s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::DoubleHolder* holder = new TIDorb::core::util::DoubleHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_longdouble(CORBA::LongDouble& value) const
{
  if (m_effective_type->kind() != CORBA::tk_longdouble)
    return false;
  // skip optimization
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  TIDorb::core::util::LongDoubleHolder* holder =
    (TIDorb::core::util::LongDoubleHolder*) (m_value);

  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_longdouble(CORBA::LongDouble s, CORBA::TypeCode_ptr type)
{
  TIDorb::core::util::LongDoubleHolder* holder = new TIDorb::core::util::LongDoubleHolder(s);

  CORBA::TypeCode_ptr type_aux;
  if (type)
    type_aux = type; // Take care, typecode is not duplicated
  else
    type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;

  if (!type)
    CORBA::release(type_aux);
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_string(const char*& value) const
{
  if (m_effective_type->kind() != CORBA::tk_string)
    return false;
  // skip optimization
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  
  
  TIDorb::core::util::StringHolder* holder =
    (TIDorb::core::util::StringHolder*) (m_value);

  if(holder) {
    value = holder->value();
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_string(const char* s)
{
  TIDorb::core::util::StringHolder* holder =
    new TIDorb::core::util::StringHolder();
  holder->value(s);

  CORBA::TypeCode_var type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;
}

void TIDorb::core::AnyImpl::insert_string(char* s)
{
  TIDorb::core::util::StringHolder* holder =
    new TIDorb::core::util::StringHolder();
  holder->value(s);

  CORBA::TypeCode_var type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_wstring(const CORBA::WChar*& value) const
{
  if (m_effective_type->kind() != CORBA::tk_wstring)
    return false;

  // skip optimization
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  
  
  TIDorb::core::util::WStringHolder* holder =
    (TIDorb::core::util::WStringHolder*) (m_value);

  if(holder) {
    value = holder->value();
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_wstring(const CORBA::WChar* s)
{
  TIDorb::core::util::WStringHolder* holder =
    new TIDorb::core::util::WStringHolder();
  holder->value(s);

  CORBA::TypeCode_var type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;
}

void TIDorb::core::AnyImpl::insert_wstring(CORBA::WChar* s)
{
  TIDorb::core::util::WStringHolder* holder =
    new TIDorb::core::util::WStringHolder();
  holder->value(s);

  CORBA::TypeCode_var type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_any(const CORBA::Any*& value) const
{
  if (m_effective_type->kind() != CORBA::tk_any)
    return false;

  // skip optimization
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  
  
  TIDorb::core::util::AnyHolder* holder =
    (TIDorb::core::util::AnyHolder*) (m_value);

  if(holder) {
    value = holder->value();
    return true;
  }

  return false;
}

//se implementa un ANY que no borre
CORBA::Boolean TIDorb::core::AnyImpl::extract_any( CORBA::Any*& value) 
{
  if (m_effective_type->kind() != CORBA::tk_any)
    return false;

  // skip optimization
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  }
  
  
  TIDorb::core::util::AnyHolder* holder =
    (TIDorb::core::util::AnyHolder*) (m_value);

  if(holder) {
     
    holder->free=false;
    value = (CORBA::Any *)holder->value();
    return true;
  }

  return false;
}



void TIDorb::core::AnyImpl::insert_any(CORBA::Any* s)
{
  TIDorb::core::util::AnyHolder* holder = new TIDorb::core::util::AnyHolder();
  holder->value(s);

  CORBA::TypeCode_var type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;
}

void TIDorb::core::AnyImpl::insert_any(const CORBA::Any& s)
{
  TIDorb::core::util::AnyHolder* holder =
    new TIDorb::core::util::AnyHolder();

  holder->value(s);


  CORBA::TypeCode_var type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_Object(CORBA::Object_out value) const
{
  if (m_effective_type->kind() != CORBA::tk_objref)
    return false;
 
  // skip optimization
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  
  
  TIDorb::core::util::ObjectHolder* holder =
    (TIDorb::core::util::ObjectHolder*) (m_value);

  if(holder) {

//    value = CORBA::Object::_duplicate(holder->value());
    value = holder->value();

    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_Object(CORBA::Object_ptr s)
{
  TIDorb::core::util::ObjectHolder* holder =
    new TIDorb::core::util::ObjectHolder();
  holder->value(s);

  CORBA::TypeCode_var type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;
}

void TIDorb::core::AnyImpl::insert_Object(CORBA::Object_ptr s, const CORBA::TypeCode_ptr tc)
{
  TIDorb::core::util::ObjectHolder* holder =
    new TIDorb::core::util::ObjectHolder();
  holder->value(s,tc);

  CORBA::TypeCode_var type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_TypeCode(CORBA::TypeCode_out value) const
{
  if (m_effective_type->kind() != CORBA::tk_TypeCode)
    return false;
 
  // skip optimization
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  
  
  TIDorb::core::util::TypeCodeHolder* holder =
    (TIDorb::core::util::TypeCodeHolder*) (m_value);

  if(holder) {
    value = CORBA::TypeCode::_duplicate(holder->value);
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_TypeCode(CORBA::TypeCode_ptr s)
{
  TIDorb::core::util::TypeCodeHolder* holder =
    new TIDorb::core::util::TypeCodeHolder(s);

  CORBA::TypeCode_var type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_Streamable(const TIDorb::portable::Streamable*& value) const
{
  if(m_wrapped_any)
    return m_wrapped_any->extract_Streamable(value);

  // skip optimization
  // if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  // {
  //   ((TIDorb::core::AnyImpl *)this)->skip_value((TIDorb::core::AnyImpl *)this)->(m_marshaled_value);
  //   ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  // } 
  
  if(m_value) {
    value = m_value;
    return true;
  }
  value = NULL;
  return false;
}

CORBA::Boolean TIDorb::core::AnyImpl::set_Streamable(TIDorb::portable::Streamable* value) const
{
  CORBA::TypeCode_var value_tc = value->_type();
  if(!m_type->equivalent(value_tc))
    return false;

  if(m_wrapped_any)
    return m_wrapped_any->set_Streamable(value);

  if(m_value)
    return false;

  ((AnyImpl*) this)->init_value(value);

  return true;
}

void TIDorb::core::AnyImpl::insert_Streamable(TIDorb::portable::Streamable* value)
{
  if(!value)
    throw CORBA::BAD_PARAM();

  CORBA::TypeCode_var type_aux = value->_type();

  set_type(type_aux);

  m_value = value;
}

void TIDorb::core::AnyImpl::insert_Streamable(const TIDorb::portable::Streamable* value)
{
  if(!value)
    throw CORBA::BAD_PARAM();

  CORBA::TypeCode_var type_aux = value->_type();

  set_type(type_aux);

  m_value = (TIDorb::portable::Streamable*) value;

  m_value->_add_ref();

}


bool TIDorb::core::AnyImpl::dump(ostream& os) const
{
  os << "[ANY]";
  m_type->dump(os);

  
  TIDorb::core::cdr::CDRInputStream* input =
    (TIDorb::core::cdr::CDRInputStream*) (create_input_stream());

  bool result = m_type->dump_value(*input, os);

  delete input;

  return result;
}

void TIDorb::core::AnyImpl::insert_Value(CORBA::ValueBase* value)
{
  TIDorb::core::util::ValueHolder* holder =
    new TIDorb::core::util::ValueHolder();
  holder->value(value);

  CORBA::TypeCode_var type_aux = holder->_type();

  set_type(type_aux);

  m_value = holder;
}

void TIDorb::core::AnyImpl::insert_Value(CORBA::ValueBase* the_value, CORBA::TypeCode_ptr the_type)
{
  TIDorb::core::util::ValueHolder* holder =
    new TIDorb::core::util::ValueHolder();
  holder->value(the_value);

  set_type(the_type);
  m_value = holder;
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_Value(CORBA::ValueBase*& value) const
{
  if (m_effective_type->kind() != CORBA::tk_value)
    return false;

  // skip optimization
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  
  
  TIDorb::core::util::ValueHolder* holder =
    (TIDorb::core::util::ValueHolder*) (m_value);

  if(holder) { 
    
    value = holder->value();
        
    // TODO: improve performance 
    TIDorb::core::util::GenericValue* generic_value =
     dynamic_cast<TIDorb::core::util::GenericValue*> (value);   
     
    if(generic_value) {
        value->_remove_ref();
        return false;
    }
            
    // memory leak: value() calls _add_ref();
    //value->_add_ref();    
    return true;
  }

  return false;
}

void TIDorb::core::AnyImpl::insert_fixed (const CORBA::Fixed& value, CORBA::UShort digits, CORBA::Short scale)
{
  CORBA::TypeCode_var type = TIDorb::portable::TypeCodeFactory::create_fixed_tc(digits, scale);
  insert_Streamable(new TIDorb::core::util::FixedHolder(type, value));
}

void TIDorb::core::AnyImpl::insert_fixed (const CORBA::Fixed& value, CORBA::TypeCode_ptr type)
{
  insert_Streamable(new TIDorb::core::util::FixedHolder(type, value));
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_fixed(CORBA::Fixed& value, CORBA::TypeCode_ptr type) const
{
  if(!type->equal(m_type))
    return false;

  // skip optimization
  if (!m_value&&mi_cdr!=TIDorb::core::AnyImpl::OK)
  {
    ((TIDorb::core::AnyImpl *)this)->skip_value(((TIDorb::core::AnyImpl *)this)->m_marshaled_value);
    ((TIDorb::core::AnyImpl *)this)->mi_cdr=TIDorb::core::AnyImpl::OK;
  } 
  
  
  TIDorb::core::util::FixedHolder* holder =
    (TIDorb::core::util::FixedHolder*) (m_value);

  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}

CORBA::Boolean TIDorb::core::AnyImpl::extract_fixed(CORBA::Fixed& value,
                                                    CORBA::UShort digits,
                                                    CORBA::Short  scale) const
{
  // TODO: validar digitos y escala ???????
  if(m_effective_type->kind() != CORBA::tk_fixed)
    return false;
  
  
  TIDorb::core::util::FixedHolder* holder =
    (TIDorb::core::util::FixedHolder*) (m_value);

  if(holder) {
    value = holder->value;
    return true;
  }

  return false;
}
