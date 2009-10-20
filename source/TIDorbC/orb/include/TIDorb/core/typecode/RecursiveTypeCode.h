/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        RecursiveTypeCode.h
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core/typecode.h"

#ifndef _TIDorb_core_typecode_RecursiveTypeCode_H_
#define _TIDorb_core_typecode_RecursiveTypeCode_H_

#include "CORBA.h"
#include "TIDThr.h"


namespace TIDorb {
namespace core {
namespace typecode {


class RecursiveTypeCode : public TypeCodeImpl
{
  public:
    RecursiveTypeCode()
       throw (TIDThr::SystemException);

    RecursiveTypeCode(const char* id)
       throw (TIDThr::SystemException);

    ~RecursiveTypeCode()
       throw (TIDThr::SystemException);

    CORBA::Boolean equal(CORBA::TypeCode_ptr) const;

    const char* id() const;

    // TIDorb operations

    bool isSimple()const;

    /**
    * TypeCode marshalling.
    */

    //void marshal(TIDorb::core::cdr::CDROutputStream& output);

    void partial_unmarshal(TIDorb::core::cdr::CDRInputStream& input);

//     bool skip_value(TIDorb::core::cdr::CDRInputStream& input) const;

//     void remarshal_value(TIDorb::core::cdr::CDRInputStream& input,
//                          TIDorb::core::cdr::CDROutputStream& output) const;

//     bool values_equal (TIDorb::core::cdr::CDRInputStream& a_input,
//                        TIDorb::core::cdr::CDRInputStream& b_input) const;

    void dump (ostream& output) const;

    bool dump_value(TIDorb::core::cdr::CDRInputStream& input, ostream& output) const;

  protected:

    char* m_id;

};


}
}
}

#endif
