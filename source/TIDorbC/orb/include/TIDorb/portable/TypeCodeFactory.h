/*/////////////////////////////////////////////////////////////////////////
//
// File:        TypeCodeFactory.h
// Description: 
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////*/

#include "CORBA.h"


#ifndef _TIDorb_portable_TypeCodeFactory_H_
#define _TIDorb_portable_TypeCodeFactory_H_

namespace TIDorb {
namespace portable {

class TypeCodeFactory {

  public:

    static ::CORBA::TypeCode_ptr
      create_struct_tc(const char* id,
                       const char* name,
                       ::CORBA::StructMemberSeq_ptr members);

    static ::CORBA::TypeCode_ptr
      create_union_tc(const char* id,
                      const char* name,
                      ::CORBA::TypeCode_ptr discriminator_type,
                      ::CORBA::UnionMemberSeq_ptr members);

    static ::CORBA::TypeCode_ptr
      create_enum_tc(const char* id,
                     const char* name,
                     ::CORBA::EnumMemberSeq_ptr members);

    static ::CORBA::TypeCode_ptr
      create_alias_tc(const char* id,
                      const char* name,
                      ::CORBA::TypeCode_ptr original_type);

    static ::CORBA::TypeCode_ptr
      create_exception_tc(const char* id,
                          const char* name,
                          ::CORBA::StructMemberSeq_ptr members);

    static ::CORBA::TypeCode_ptr
      create_interface_tc(const char* id,
                          const char* name);

    static ::CORBA::TypeCode_ptr
      create_string_tc(::CORBA::ULong bound);

    static ::CORBA::TypeCode_ptr
      create_wstring_tc(::CORBA::ULong bound);

    static ::CORBA::TypeCode_ptr
      create_fixed_tc(::CORBA::UShort digits, ::CORBA::Short scale);

    static ::CORBA::TypeCode_ptr
      create_sequence_tc(::CORBA::ULong bound, ::CORBA::TypeCode_ptr element_type);

    // deprecated
    static ::CORBA::TypeCode_ptr
      create_recursive_sequence_tc(::CORBA::ULong bound, ::CORBA::ULong offset);

    static ::CORBA::TypeCode_ptr
      create_array_tc(::CORBA::ULong length,
                      ::CORBA::TypeCode_ptr element_type);

    static ::CORBA::TypeCode_ptr
      create_value_tc(const char* id,
                      const char* name,
                      ::CORBA::ValueModifier type_modifier,
                      ::CORBA::TypeCode_ptr concrete_base,
                      ::CORBA::ValueMemberSeq_ptr members);

    static ::CORBA::TypeCode_ptr
      create_value_box_tc(const char* id,
                          const char* name,
                          ::CORBA::TypeCode_ptr boxed_type);

    static ::CORBA::TypeCode_ptr
      create_native_tc(const char* id,
                       const char* name);

    static ::CORBA::TypeCode_ptr
      create_recursive_tc(const char* id);

    static ::CORBA::TypeCode_ptr
      create_abstract_interface_tc(const char* id,
                                   const char* name);

    static ::CORBA::TypeCode_ptr
      create_local_interface_tc(const char* id,
                                const char* name);

    static CORBA::TypeCode_ptr
      get_basic_TypeCode(CORBA::TCKind tcKind);
};

} //portable
} //TIDorb



#endif
