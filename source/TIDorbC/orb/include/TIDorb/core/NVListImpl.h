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
