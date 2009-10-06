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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Righ Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
