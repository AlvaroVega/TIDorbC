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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
