//////////////////////////////////////////////////////////////////////////////////
//
// File:        ContextListImpl.h
// Description: CORBA::ContextList implementation.
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

#ifndef __TIDorb_core_ContextList_H_
#define __TIDorb_core_ContextList_H_

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
  
  class ContextListImpl : public CORBA::ContextList,
                          public TIDThr::RecursiveMutex
{   
  public:
    ContextListImpl()  throw (TIDThr::SystemException);
    ~ContextListImpl()  throw (TIDThr::SystemException);
    
    CORBA::ULong count() const;
    void add(const char* ctxt);
    void add_consume(char* ctxt);
    const char* item(CORBA::ULong index);
    void remove(CORBA::ULong index);
    
  protected:
  
    typedef vector<char*> ItemList;
    
    ItemList m_list;
};

}
}

#endif
