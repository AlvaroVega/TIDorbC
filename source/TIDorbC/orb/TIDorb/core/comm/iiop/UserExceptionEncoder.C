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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
//PRA
  in.read_string(name.out());
//EPRA

  try {
    if (types != NULL) {
      CORBA::TypeCode_var type;

      for (CORBA::ULong i = 0; i < types->count(); i++) //ATENCION -MCPG
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


