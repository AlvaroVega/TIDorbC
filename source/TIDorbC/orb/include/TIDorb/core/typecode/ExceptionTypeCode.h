/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        ExceptionTypeCode.h
// Description: TypeCode implementation for complex exceptions.
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

#ifndef _TIDorb_core_typecode_ExceptionTypeCode_H_
#define _TIDorb_core_typecode_ExceptionTypeCode_H_

#include "CORBA.h"
#include "TIDThr.h"

#ifdef TIDORB_HAVE_IOSTREAM
  #include <iostream>
#else
  #include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif


namespace TIDorb {
namespace core {
namespace typecode {


class ExceptionTypeCode : public StructTypeCode
{
  public:
    ExceptionTypeCode()
       throw (TIDThr::SystemException);

    ExceptionTypeCode(const char* id, const char* name, CORBA::StructMemberSeq* members)
       throw (TIDThr::SystemException);

    /**
    * TypeCode marshalling.
    */

    bool skip_value(TIDorb::core::cdr::CDRInputStream& input) const;

    void remarshal_value(TIDorb::core::cdr::CDRInputStream& input,
                         TIDorb::core::cdr::CDROutputStream& output) const;

    bool values_equal (TIDorb::core::cdr::CDRInputStream& a_input,
                       TIDorb::core::cdr::CDRInputStream& b_input) const;

    void dump (ostream& output) const;

    bool dump_value(TIDorb::core::cdr::CDRInputStream& input, ostream& output) const;

};


}
}
}

#endif

