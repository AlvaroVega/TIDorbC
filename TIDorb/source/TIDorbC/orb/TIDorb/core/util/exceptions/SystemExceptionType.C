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

#include "TIDorb/core/util/exceptions.h"


::CORBA::TypeCode_ptr
TIDorb::core::util::exceptions::SystemExceptionType::create(const char* id)
{     
    CORBA::StructMemberSeq_ptr members = new ::CORBA::StructMemberSeq();
    
    members->length(2);
    
    (*members)[0].name = (::CORBA::Identifier) CORBA::string_dup("minor");
    (*members)[0].type = TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(::CORBA::tk_ulong);

    (*members)[0].type_def = 0;
    (*members)[1].name = (::CORBA::Identifier) CORBA::string_dup("completed");
    (*members)[1].type = CORBA::TypeCode::_duplicate(CORBA::_CompletionStatusHelper::type());

    (*members)[1].type_def = 0;
    return TIDorb::portable::TypeCodeFactory::create_exception_tc(id, "BAD_PARAM", members);
}
