/////////////////////////////////////////////////////////////////////////
//
// File:        SystemExceptionType.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/util/exceptions.h"


::CORBA::TypeCode_ptr
TIDorb::core::util::exceptions::SystemExceptionType::create(const char* id)
{     
    CORBA::StructMemberSeq_ptr members = new ::CORBA::StructMemberSeq();
    
    members->length(2);
    
    (*members)[0].name = (::CORBA::Identifier) CORBA::string_dup("minor");
    (*members)[0].type = TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_ulong);
    //jagd 
    //(*members)[0].type_def = CORBA::IDLType::_nil();
    (*members)[0].type_def = 0;
    (*members)[1].name = (::CORBA::Identifier) CORBA::string_dup("completed");
    (*members)[1].type = CORBA::TypeCode::_duplicate(CORBA::_tc_CompletionStatus);
    //jagd 
    //(*members)[1].type_def = CORBA::IDLType::_nil();
    (*members)[1].type_def = 0;
    return TIDorb::portable::TypeCodeFactory::create_exception_tc(id, "BAD_PARAM", members);
}
