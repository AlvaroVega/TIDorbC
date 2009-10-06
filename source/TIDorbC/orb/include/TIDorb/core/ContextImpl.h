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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

