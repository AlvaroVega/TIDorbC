/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        NativeTypeCode.h
// Description: TypeCode implementation for native interfaces.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Righ Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core/typecode.h"

#ifndef _TIDorb_core_typecode_NativeTypeCode_H_
#define _TIDorb_core_typecode_NativeTypeCode_H_

#include "CORBA.h"
#include "TIDThr.h"


namespace TIDorb {
namespace core {
namespace typecode {


class NativeTypeCode : public ComplexTypeCode
{
  public:
    NativeTypeCode()
       throw (TIDThr::SystemException)
       : ComplexTypeCode(CORBA::tk_native)
    {
    }

    NativeTypeCode(const char* repositoryId, const char* name)
       throw (TIDThr::SystemException)
       : ComplexTypeCode(CORBA::tk_native, repositoryId, name)
    {
    }
};


}
}
}

#endif
