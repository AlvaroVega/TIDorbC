//////////////////////////////////////////////////////////////////////////////////
//
// File:        NVListImpl.h
// Description: CORBA::NVList implementation.
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

#include "TIDorb/core.h"

#ifndef _TIDorb_core_NVList_H_
#define _TIDorb_core_NVList_H_

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
  

  class NVListImpl: public CORBA::NVList //,
                 //jagd 2 public TIDThr::RecursiveMutex
{
  public:
    NVListImpl(){};
 
    NVListImpl(TIDORB* orb)  throw (TIDThr::SystemException);
    ~NVListImpl()  throw (TIDThr::SystemException);
  
    CORBA::ULong count() const;
    CORBA::NamedValue_ptr add(CORBA::Flags flags);
    CORBA::NamedValue_ptr add_item(const char* item_name, CORBA::Flags flags);
    CORBA::NamedValue_ptr add_value(const char* item_name, const CORBA::Any& value, CORBA::Flags flags);
    CORBA::NamedValue_ptr add_item_consume(char* item_name, CORBA::Flags flags);
    CORBA::NamedValue_ptr add_value_consume(char* item_name,CORBA::Any* value, CORBA::Flags flags);
    CORBA::NamedValue_ptr add_value_consume(const char* item_name,CORBA::Any* value, CORBA::Flags flags);
    CORBA::NamedValue_ptr item(CORBA::ULong index);
    void remove(CORBA::ULong index);
//MCPG
    static void read_params_out(CORBA::NVList_ptr list, TIDorb::core::cdr::CDRInputStream& input);
    static void write_params_out(CORBA::NVList_ptr list, TIDorb::core::cdr::CDROutputStream& output);
    static void read_params_in(CORBA::NVList_ptr list, TIDorb::core::cdr::CDRInputStream& input);
    static void write_params_in(CORBA::NVList_ptr list, TIDorb::core::cdr::CDROutputStream& output);
                   
    static void assign_in_arguments(CORBA::NVList_ptr from_list, 
                                    CORBA::NVList_ptr to_list);
                                    
    static void assign_in_arguments(CORBA::NVList_ptr from_list, 
                                    CORBA::NVList_ptr to_list,
                                    bool wrap_anys);
                                    
    static void assign_out_arguments(CORBA::NVList_ptr from_list, 
                                     CORBA::NVList_ptr to_list);
                                    
    static void assign_out_arguments(CORBA::NVList_ptr from_list, 
                                    CORBA::NVList_ptr to_list,
                                    bool wrap_anys);                                                                 
    
  protected:
  
    typedef vector<CORBA::NamedValue_ptr> ItemList;
    
    ItemList m_list;
    TIDORB* m_orb;

  public:
    void* _impl();
    const char* _typeid();

};

}
}

#endif
