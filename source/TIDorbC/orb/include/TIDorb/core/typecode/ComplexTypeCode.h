/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        ComplexCodeImpl.h
// Description: TypeCode implementation base class for complex typecodes such as structs.
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

#ifndef _TIDorb_core_typecode_ComplexTypeCode_H_
#define _TIDorb_core_typecode_ComplexTypeCode_H_

#include "CORBA.h"
#include "TIDThr.h"
//MLG
#include "TIDorb/core/typecode/TypeCodeCache.h"
//EMLG

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


class ComplexTypeCode : public TypeCodeImpl
{
  public:
    ComplexTypeCode(CORBA::TCKind kind)
       throw (TIDThr::SystemException);

    ComplexTypeCode(CORBA::TCKind kind, const char* repositoryId, const char* name)
       throw (TIDThr::SystemException);

    virtual ~ComplexTypeCode()
       throw (TIDThr::SystemException);

    virtual CORBA::Boolean equal(CORBA::TypeCode_ptr) const;
    virtual CORBA::Boolean equivalent(CORBA::TypeCode_ptr) const;

    const char* id() const;
    const char* name() const;

    // TIDorb operations

    bool isSimple()const;

    /**
     * TypeCode marshalling
     */

    virtual void marshal(TIDorb::core::cdr::CDROutputStream& output);
    void partial_unmarshal(TIDorb::core::cdr::CDRInputStream& input);


    /**
     * Complex TC operations
     */

    virtual void write_params(TIDorb::core::cdr::CDROutputStream& output);
    virtual void read_params(TIDorb::core::cdr::CDRInputStream& input);

  protected:
    void dump_params (ostream& output) const;

  protected:
    char* m_name;
    char* m_id;
    TIDorb::core::cdr::BufferCDR_ref m_marshaled_params;
    TIDThr::RecursiveMutex m_recursive_mutex;

};


}
}
}

#endif
