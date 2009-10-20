/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        EnumTypeCode.h
// Description: TypeCode implementation for enums.
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core/typecode.h"

#ifndef _TIDorb_core_typecode_EnumTypeCode_H_
#define _TIDorb_core_typecode_EnumTypeCode_H_

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


class EnumTypeCode : public ComplexTypeCode
{
  public:
    EnumTypeCode()
       throw (TIDThr::SystemException);

    //PRA
    //EnumTypeCode(const char* id, const char* name, const CORBA::EnumMemberSeq* members)
    EnumTypeCode(const char* id, const char* name, CORBA::EnumMemberSeq* members)
    //EPRA
       throw (TIDThr::SystemException);

    virtual ~EnumTypeCode()
       throw (TIDThr::SystemException);

    CORBA::Boolean equal(CORBA::TypeCode_ptr tc) const;
    CORBA::Boolean equivalent(CORBA::TypeCode_ptr) const;
    
    CORBA::ULong member_count() const;
    const char* member_name(CORBA::ULong index) const;


    /**
    * TypeCode marshalling.
    */

    bool skip_value(TIDorb::core::cdr::CDRInputStream& input) const;

    void remarshal_value(TIDorb::core::cdr::CDRInputStream& input,
                         TIDorb::core::cdr::CDROutputStream& output) const;

    bool values_equal (TIDorb::core::cdr::CDRInputStream& a_input,
                       TIDorb::core::cdr::CDRInputStream& b_input) const;

    void dump (ostream& output) const;

    bool dump_value(TIDorb::core::cdr::CDRInputStream& input,  ostream& output) const;

    void write_params(TIDorb::core::cdr::CDROutputStream& output);
    void read_params(TIDorb::core::cdr::CDRInputStream& input);

  protected:
    CORBA::EnumMemberSeq* m_members;
};


}
}
}

#endif

