//////////////////////////////////////////////////////////////////////////////////
//
// File:        UserExceptionEnconder.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/iiop/UserExceptionEncoder.h"




void TIDorb::core::comm::iiop::UserSystemExceptionEncoder::write(
	TIDorb::portable::OutputStream& out, CORBA::SystemException& sys)
{
	out.write_string(sys._name());
#ifdef minor
#define __CORBA_SYSTEM_EXCEPTION_MINOR_ minor
#undef minor
#endif
	out.write_ulong(sys.minor());
#ifdef __CORBA_SYSTEM_EXCEPTION_MINOR__
#define minor __CORBA_SYSTEM_EXCEPTION_MINOR__
#undef __CORBA_SYSTEM_EXCEPTION_MINOR__
#endif
	out.write_ulong(sys.completed());
};
	
CORBA::UnknownUserException* TIDorb::core::comm::iiop::UserSystemExceptionEncoder::read(
TIDorb::portable::InputStream& in, CORBA::ExceptionList_ptr types)
{
  CORBA::String_var name;

  in.read_string(name.out());

  try {
    if (types != NULL) {
      CORBA::TypeCode_var type;

      for (CORBA::ULong i = 0; i < types->count(); i++) 
      {
        type = types->item(i);
        if(type->kind() != CORBA::tk_except)
        throw CORBA::BAD_PARAM();//"Bad ExceptionList: not Exception TypeCode");

        if(strcmp(name,type->name())==0) {
          CORBA::Any* any = in.orb()->create_any();
          //any->read_value(in, types[i]);
          any->delegate().read_value(in, type);
          return new CORBA::UnknownUserException(*any);
        }
      }
    }
  }catch (const CORBA::TypeCode::BadKind& bk) {}

  throw CORBA::UNKNOWN(name);
  
};


