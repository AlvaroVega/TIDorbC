/////////////////////////////////////////////////////////////////////////
//
// File:        TypeCodeFactory.C
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

static TIDorb::core::typecode::StringTypeCode _aux_tc_string;
const CORBA::TypeCode_ptr CORBA::_tc_string= (CORBA::TypeCode_ptr) &_aux_tc_string;

static TIDorb::core::typecode::WStringTypeCode _aux_tc_wstring;
const CORBA::TypeCode_ptr CORBA::_tc_wstring= (CORBA::TypeCode_ptr) &_aux_tc_wstring;

static TIDorb::core::typecode::ObjectRefTypeCode _aux_tc_Object("IDL:omg.org/CORBA/Object:1.0", "Object");
const CORBA::TypeCode_ptr CORBA::_tc_Object= (CORBA::TypeCode_ptr) &_aux_tc_Object;

static TIDorb::core::typecode::ValueTypeCode _aux_tc_Value;
const CORBA::TypeCode_ptr CORBA::_tc_Value= (CORBA::TypeCode_ptr) &_aux_tc_Value;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_TypeCode(CORBA::tk_TypeCode);
const CORBA::TypeCode_ptr CORBA::_tc_TypeCode= (CORBA::TypeCode_ptr) &_aux_tc_TypeCode;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_null(CORBA::tk_null);
const CORBA::TypeCode_ptr CORBA::_tc_null= (CORBA::TypeCode_ptr) &_aux_tc_null;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_void(CORBA::tk_void);
const CORBA::TypeCode_ptr CORBA::_tc_void= (CORBA::TypeCode_ptr) &_aux_tc_void;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_octet(CORBA::tk_octet);
const CORBA::TypeCode_ptr CORBA::_tc_octet= (CORBA::TypeCode_ptr) &_aux_tc_octet;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_boolean(CORBA::tk_boolean);
const CORBA::TypeCode_ptr CORBA::_tc_boolean= (CORBA::TypeCode_ptr) &_aux_tc_boolean;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_char(CORBA::tk_char);
const CORBA::TypeCode_ptr CORBA::_tc_char= (CORBA::TypeCode_ptr) &_aux_tc_char;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_wchar(CORBA::tk_wchar);
const CORBA::TypeCode_ptr CORBA::_tc_wchar= (CORBA::TypeCode_ptr) &_aux_tc_wchar;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_short(CORBA::tk_short);
const CORBA::TypeCode_ptr CORBA::_tc_short= (CORBA::TypeCode_ptr) &_aux_tc_short;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_ushort(CORBA::tk_ushort);
const CORBA::TypeCode_ptr CORBA::_tc_ushort= (CORBA::TypeCode_ptr) &_aux_tc_ushort;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_long(CORBA::tk_long);
const CORBA::TypeCode_ptr CORBA::_tc_long= (CORBA::TypeCode_ptr) &_aux_tc_long;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_ulong(CORBA::tk_ulong);
const CORBA::TypeCode_ptr CORBA::_tc_ulong= (CORBA::TypeCode_ptr) &_aux_tc_ulong;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_longlong(CORBA::tk_longlong);
const CORBA::TypeCode_ptr CORBA::_tc_longlong= (CORBA::TypeCode_ptr) &_aux_tc_longlong;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_ulonglong(CORBA::tk_ulonglong);
const CORBA::TypeCode_ptr CORBA::_tc_ulonglong= (CORBA::TypeCode_ptr) &_aux_tc_ulonglong;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_float(CORBA::tk_float);
const CORBA::TypeCode_ptr CORBA::_tc_float= (CORBA::TypeCode_ptr) &_aux_tc_float;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_double(CORBA::tk_double);
const CORBA::TypeCode_ptr CORBA::_tc_double= (CORBA::TypeCode_ptr) &_aux_tc_double;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_longdouble(CORBA::tk_longdouble);
const CORBA::TypeCode_ptr CORBA::_tc_longdouble= (CORBA::TypeCode_ptr) &_aux_tc_longdouble;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_any(CORBA::tk_any);
const CORBA::TypeCode_ptr CORBA::_tc_any= (CORBA::TypeCode_ptr) &_aux_tc_any;

static TIDorb::core::typecode::TypeCodeImpl _aux_tc_principal(CORBA::tk_Principal);
const CORBA::TypeCode_ptr CORBA::_tc_principal= (CORBA::TypeCode_ptr) &_aux_tc_principal;

::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_struct_tc(const char* id,
                                                    const char* name,
                                                    ::CORBA::StructMemberSeq_ptr members)
{
  if((!id) || (!name) || (!members))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::StructTypeCode(id, name, members);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_union_tc(const char* id,
                                                   const char* name,
                                                   ::CORBA::TypeCode_ptr discriminator_type,
                                                   ::CORBA::UnionMemberSeq_ptr members)
{
  if((!id) || (!name) || (!members) || !(discriminator_type))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::UnionTypeCode(id, name, discriminator_type, members);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_enum_tc(const char* id,
                                                  const char* name,
                                                  ::CORBA::EnumMemberSeq_ptr members)
{
  if((!id) || (!name) || (!members))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::EnumTypeCode(id, name, members);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_alias_tc(const char* id,
                                                   const char* name,
                                                   ::CORBA::TypeCode_ptr original_type)
{
  if((!id) || (!name) || (!(original_type)))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::AliasTypeCode(id, name, original_type);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_exception_tc(const char* id,
                                                       const char* name,
                                                       ::CORBA::StructMemberSeq_ptr members)
{
  if((!id) || (!name) || (!members))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::ExceptionTypeCode(id, name, members);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_interface_tc(const char* id,
                                                       const char* name)
{
  if((!id) || (!name))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::ObjectRefTypeCode(id, name);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_string_tc(::CORBA::ULong bound)
{
  return new TIDorb::core::typecode::StringTypeCode(bound);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_wstring_tc(::CORBA::ULong bound)
{
  return new TIDorb::core::typecode::WStringTypeCode(bound);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_fixed_tc(::CORBA::UShort digits,
                                                   ::CORBA::Short scale)
{
  return new TIDorb::core::typecode::FixedTypeCode(digits, scale);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_sequence_tc(::CORBA::ULong bound,
                                                      ::CORBA::TypeCode_ptr element_type)
{
  if(!(element_type))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::SequenceTypeCode(element_type, bound);
}


// deprecated
::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_recursive_sequence_tc(::CORBA::ULong bound,
                                                                ::CORBA::ULong offset)
{
  throw CORBA::NO_IMPLEMENT();
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_array_tc(::CORBA::ULong length,
                                                   ::CORBA::TypeCode_ptr element_type)
{
  if(!(element_type))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::ArrayTypeCode(element_type, length);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_value_tc(const char* id,
                                                   const char* name,
                                                   ::CORBA::ValueModifier type_modifier,
                                                   ::CORBA::TypeCode_ptr concrete_base,
                                                   ::CORBA::ValueMemberSeq_ptr members)
{
 if((!id) || (!name) || (!members)) /* || (CORBA::is_nil(concrete_base))) */
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::ValueTypeCode(id, name, type_modifier, concrete_base, members);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_value_box_tc(const char* id,
                                                       const char* name,
                                                       ::CORBA::TypeCode_ptr boxed_type)
{
 if((!id) || (!name) || (!(boxed_type)))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::ValueBoxTypeCode(id, name, boxed_type);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_native_tc(const char* id,
                                                    const char* name)
{
  if((!id) || (!name))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::NativeTypeCode(id, name);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_recursive_tc(const char* id)
{
  if(!id) 
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::RecursiveTypeCode(id);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_abstract_interface_tc(const char* id,
                                                                const char* name)
{
   if((!id) || (!name))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::AbstractInterfaceTypeCode(id, name);
}


::CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::create_local_interface_tc(const char* id,
                                                             const char* name)
{
   if((!id) || (!name))
    throw CORBA::BAD_PARAM();

  return new TIDorb::core::typecode::LocalInterfaceTypeCode(id, name);
}


CORBA::TypeCode_ptr
TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(CORBA::TCKind tcKind)
{
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_null =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_any =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_TypeCode =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_void =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_short =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_long =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_ushort =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_ulong =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_float =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_double =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_boolean =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_char =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_octet =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_longlong =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_ulonglong =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_longdouble =NULL; 
   static TIDorb::core::typecode::TypeCodeImpl * typeCode_tk_wchar =NULL; 







  switch (tcKind) {
    case CORBA::tk_null:
        if(typeCode_tk_null==NULL)
        {
          typeCode_tk_null= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_null);
        }
        return typeCode_tk_null; 
    case CORBA::tk_objref:
        return new TIDorb::core::typecode::ObjectRefTypeCode("IDL:omg.org/CORBA/Object:1.0", "Object");
    case CORBA::tk_value:
        return new TIDorb::core::typecode::ValueTypeCode("IDL:omg.org/CORBA/ValueBase:1.0", 
                                                         "ValueBase", 0, 0, 
                                                         new CORBA::ValueMemberSeq(0));
    case CORBA::tk_any:
        if(typeCode_tk_any==NULL)
        {
          typeCode_tk_any= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_any);
        }
      return typeCode_tk_any; 
    case CORBA::tk_TypeCode:
        if(typeCode_tk_TypeCode==NULL)
        {
          typeCode_tk_TypeCode= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_TypeCode);
        }
        return typeCode_tk_TypeCode; 
    case CORBA::tk_void:
        if(typeCode_tk_void==NULL)
        {
          typeCode_tk_void= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_void);
        }
        return typeCode_tk_void; 
    case CORBA::tk_short:
        if(typeCode_tk_short==NULL)
        {
          typeCode_tk_short= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_short);
        }
        return typeCode_tk_short; 
    case CORBA::tk_long:
        if(typeCode_tk_long==NULL)
        {
          typeCode_tk_long= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_long);
        }
        return typeCode_tk_long; 
    case CORBA::tk_ushort:
        if(typeCode_tk_ushort==NULL)
        {
          typeCode_tk_ushort= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_ushort);
        }
        return typeCode_tk_ushort; 
    case CORBA::tk_ulong:
        if(typeCode_tk_ulong==NULL)
        {
          typeCode_tk_ulong= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_ulong);
        }
        return typeCode_tk_ulong; 
    case CORBA::tk_float:
        if(typeCode_tk_float==NULL)
        {
          typeCode_tk_float= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_float);
        }
        return typeCode_tk_float; 
    case CORBA::tk_double:
        if(typeCode_tk_double==NULL)
        {
          typeCode_tk_double= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_double);
        }
        return typeCode_tk_double; 
    case CORBA::tk_boolean:
        if(typeCode_tk_boolean==NULL)
        {
          typeCode_tk_boolean= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_boolean);
        }
        return typeCode_tk_boolean; 
    case CORBA::tk_char:
        if(typeCode_tk_char==NULL)
        {
          typeCode_tk_char= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_char);
        }
        return typeCode_tk_char; 
    case CORBA::tk_octet:
        if(typeCode_tk_octet==NULL)
        {
          typeCode_tk_octet= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_octet);
        }
        return typeCode_tk_octet; 
    case CORBA::tk_longlong:
        if(typeCode_tk_longlong==NULL)
        {
          typeCode_tk_longlong= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_longlong);
        }
        return typeCode_tk_longlong; 
    case CORBA::tk_ulonglong:
        if(typeCode_tk_ulonglong==NULL)
        {
          typeCode_tk_ulonglong= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_ulonglong);
        }
        return typeCode_tk_ulonglong; 
    case CORBA::tk_longdouble:
        if(typeCode_tk_longdouble==NULL)
        {
          typeCode_tk_longdouble= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_longdouble);
        }
        return typeCode_tk_longdouble; 
    case CORBA::tk_wchar:
        if(typeCode_tk_wchar==NULL)
        {
          typeCode_tk_wchar= new TIDorb::core::typecode::TypeCodeImpl(CORBA::tk_wchar);
        }
        return typeCode_tk_wchar; 
    case CORBA::tk_string:
        return new TIDorb::core::typecode::StringTypeCode();
    case CORBA::tk_wstring:
        return new TIDorb::core::typecode::WStringTypeCode();
    default:
        return 0;
  }
}


TIDorb::core::typecode::TypeCodeImpl*
TIDorb::core::typecode::TypeCodeFactory::get_empty_TypeCode(CORBA::TCKind tcKind)
{

  switch(tcKind) {
    case CORBA::tk_struct:
        return new StructTypeCode();
    case CORBA::tk_objref:
        return new ObjectRefTypeCode();
    case CORBA::tk_union:
        return new UnionTypeCode();
    case CORBA::tk_enum:
        return new EnumTypeCode(); 
    case CORBA::tk_string:
        return new StringTypeCode(); 
    case CORBA::tk_sequence:
        return new SequenceTypeCode();
    case CORBA::tk_array:
        return new ArrayTypeCode ();
    case CORBA::tk_alias:
        return new AliasTypeCode ();
     case CORBA::tk_except:
        return new ExceptionTypeCode();   
    case CORBA::tk_wstring:
        return new WStringTypeCode ();        
    case CORBA::tk_fixed:
        return new FixedTypeCode();
    case CORBA::tk_value:
        return new ValueTypeCode(); 
    case CORBA::tk_value_box:
        return new ValueBoxTypeCode();
    case CORBA::tk_native:
        return new NativeTypeCode (); 
    case CORBA::tk_abstract_interface:
        return new AbstractInterfaceTypeCode();
    default:
      return 0;
  }
}
