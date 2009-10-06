//////////////////////////////////////////////////////////////////////////////////
//
// File:        ExceptionListImpl.h
// Description: CORBA::ExceptionList implementation.
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

#ifndef __TIDorb_core_ExceptionListImpl_H_
#define __TIDorb_core_ExceptionListImpl_H_

#include "CORBA.h"
#include "TIDThr.h"

#include <vector>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::vector;
#endif

namespace TIDorb {
namespace core {
  
class ExceptionListImpl : public CORBA::ExceptionList,
                          public TIDThr::Mutex
{   
  public:
    ExceptionListImpl()  throw (TIDThr::SystemException);
    ~ExceptionListImpl()  throw (TIDThr::SystemException);
    
    CORBA::ULong count() const;
    void add(CORBA::TypeCode_ptr tc);
    void add_consume(CORBA::TypeCode_ptr tc);
    CORBA::TypeCode_ptr item(CORBA::ULong index);
    void remove(CORBA::ULong index);
    
  protected:
  
//FRAN
    typedef vector<CORBA::TypeCode_ptr> ItemList;
//EFRAN    
    ItemList m_list;
};

}
}

#endif
