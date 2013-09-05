//////////////////////////////////////////////////////////////////////////////////
//
// File:        ContextImpl.h
// Description: CORBA::Context implementation.
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

#ifndef __TIDorb_core_ContextImpl_H_
#define __TIDorb_core_ContextImpl_H_

#include "CORBA.h"
#include "TIDThr.h"

#include <list>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::list;
#endif

namespace TIDorb {
namespace core {

class ContextImpl : public CORBA::Context,
                    public TIDThr::RecursiveMutex
{
  public:
  
    ContextImpl(CORBA::ORB_ptr orb, const char* name, CORBA::Context_ptr parent = NULL)  throw (TIDThr::SystemException);
 
    ~ContextImpl()  throw (TIDThr::SystemException);
  
    const char *context_name() const;
    CORBA::Context_ptr parent() const;
    void create_child(const char* child_context_name, CORBA::Context_out child_context);
    void set_one_value(const char* prop_name, const CORBA::Any& value);
    void set_values(CORBA::NVList_ptr values);
    void delete_values(const char* prop_name);
    void get_values(const char* prop_name, CORBA::Flags flags, 
                    const char* patter, CORBA::NVList_out values);

    static CORBA::Context_ptr read(TIDorb::portable::InputStream& input);

    static void write(TIDorb::portable::OutputStream& output,
                      const CORBA::Context_ptr ctx, 
                      const CORBA::ContextList_ptr contexts);
                      
  protected:
    
    void get_values_aux(const char* prop_name, CORBA::Flags flags, 
                        const char* patter, CORBA::NVList_ptr values);                    
    
    static bool match_pattern(const char* name, const char* pattern); 
                      
    typedef list<ContextImpl*> ContextList;    
  
  private:
    
    CORBA::ORB_ptr m_orb;
  	
    const char* m_name;
    CORBA::Context_ptr m_parent;
    CORBA::NVList_ptr m_values;
    ContextList* m_childs;


};

}
}

#endif

