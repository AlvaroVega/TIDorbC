/////////////////////////////////////////////////////////////////////////
//
// File:        ContextImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#include <vector>
#include <string>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
using std::string;
using std::vector;
#endif

TIDorb::core::ContextImpl::ContextImpl(CORBA::ORB_ptr orb, const char* name, 
                                       CORBA::Context_ptr parent) 
  throw (TIDThr::SystemException)
: m_orb(orb),
  m_name(name), 
  //jagd 2
  //m_parent(CORBA::Context::_duplicate(parent)),
  m_parent((parent)),
  m_values(0),
  m_childs(0)
{
//jagd 3  _count(1);
}

TIDorb::core::ContextImpl::~ContextImpl() throw (TIDThr::SystemException)
{
  CORBA::release(m_orb);
  //jagd 2
  //CORBA::release(m_parent);
  //jagd 2 
  //CORBA::release(m_values);  
  delete (m_values);  
  
  if(m_childs) {
    ContextList::iterator i;
    ContextList::iterator end = m_childs->end();
    
    for(i= m_childs->begin(); i != end ; i++)
      //jagd 2 
      //CORBA::release((CORBA::Context_ptr) *i);
      delete *i;
      
    delete m_childs;
  }
}

const char* TIDorb::core::ContextImpl::context_name() const
{
  return m_name;
}

CORBA::Context_ptr TIDorb::core::ContextImpl::parent() const
{
  return m_parent;
}

void TIDorb::core::ContextImpl::create_child(const char* child_context_name, 
                                             CORBA::Context_out child_context)
{
  if(!child_context_name)
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);
		
  ContextImpl* child = 
    //jagd 2
    //new ContextImpl(m_orb, child_context_name, CORBA::Context::_duplicate(this));
    new ContextImpl(m_orb, child_context_name,(this));
	
  {
    TIDThr::Synchronized sync(*this);	
    if (!m_childs) 
      m_childs = new ContextList();
  }
	
  m_childs->push_back(child);

  child_context = child;
}                                          
                                             
void TIDorb::core::ContextImpl::set_one_value(const char* prop_name, const CORBA::Any& value)
{
  if(!prop_name)
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);

  CORBA::TypeCode_var type = value.type();
  
  if (type->kind() != CORBA::tk_string)
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO); // "Value must have a string TypeCode."

  {
    TIDThr::Synchronized sync(*this);	
          
    if (!m_values)
     m_orb->create_list(1, m_values);
  }
  
  // new value
  
  m_values->add_value(prop_name, value, 0);
}

void TIDorb::core::ContextImpl::set_values(CORBA::NVList_ptr values)
{
  TIDThr::Synchronized sync(*this);	
 
        //jagd 2 
	//if (CORBA::is_nil(values))
	if (!(values))
		throw  CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO); // "Null NVList reference"

//  jagd 2
//  CORBA::release(m_values);
  delete m_values;
  
  CORBA::ULong list_size = values->count();
  
  m_orb->create_list(list_size, m_values);

  for(CORBA::ULong i = 0; i < list_size; i++) 
    set_one_value(values->item(i)->name(), *(values->item(i)->value()));
}

void TIDorb::core::ContextImpl::delete_values(const char* prop_name)
{
  if (!prop_name)
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO); //"Null string reference"

  if (m_values) {
      CORBA::ULong list_size = m_values->count();
 
  for(int i = list_size ; i >= 0 ; i--) 
    if(!strcmp(prop_name, m_values->item(i)->name())) {
      m_values->remove(i);
    }
  }
}

void TIDorb::core::ContextImpl::get_values(const char* prop_name, CORBA::Flags flags, 
                                           const char* pattern, CORBA::NVList_out values)
{
  CORBA::NVList_ptr list; 
  m_orb->create_list(1, list);
  get_values_aux(prop_name, flags, pattern, list);
  values = list;
}
  
void TIDorb::core::ContextImpl::get_values_aux(const char* prop_name, CORBA::Flags flags, 
                                               const char* pattern, CORBA::NVList_ptr values)
{
  if ((!prop_name) || (!pattern))
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO); //"Null string reference"
  
  if(m_values && (!strcmp(prop_name, m_name))) 
  {
     CORBA::ULong list_size = m_values->count();
     for(CORBA::ULong i = 0; i < list_size; i++)
  	if(match_pattern(m_values->item(i)->name(), pattern)) 
                         values->add_value(m_values->item(i)->name(), 
			 *(m_values->item(i)->value()),0);
  }
  
  if(m_childs && (flags != CORBA::CTX_RESTRICT_SCOPE))
  {
    ContextList::iterator i;
    ContextList::iterator end = m_childs->end();
    
    for(i = m_childs->begin(); i != end; i++)
      (*i)->get_values_aux(prop_name, flags,pattern, values);
  }
}


bool TIDorb::core::ContextImpl::match_pattern(const char* name, const char* pattern) 
{
  if (!strcmp(pattern,"*"))
    return true;
  return false;
}

CORBA::Context_ptr TIDorb::core::ContextImpl::read(TIDorb::portable::InputStream& input)
{
  CORBA::ULong pair_count;
  input.read_ulong(pair_count);
  
  if (pair_count == 0)
    return 0;
  
  if ((pair_count < 1) || (pair_count%2 != 0))
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO); //"Malformed context name-value pairs",0,
                         //CompletionStatus.COMPLETED_NO);
  
  CORBA::ULong num_values = pair_count / 2;
  
  ContextImpl* context = new ContextImpl(input.orb(), "");
  
  char* name = 0;
  char* str_val = 0;
  CORBA::Any value;
  
  for(CORBA::ULong i = 0; i < num_values; i++)
  {
    input.read_string(name);
    input.read_string(str_val);
    value <<= str_val;
    context->set_one_value(name,value); 
    //free string
    CORBA::string_free(name);
  }
  
return context;
}
	
void TIDorb::core::ContextImpl::write(TIDorb::portable::OutputStream& output,
                                      const CORBA::Context_ptr ctx, 
                                      const CORBA::ContextList_ptr contexts)
{
  CORBA::ULong contexts_count = contexts->count();
  
  if(contexts_count == 0) {
    //no context
    output.write_ulong(0);
    return;
  }

  vector< const char* >  nv_context;
   //jagd2
   //CORBA::NVList_ptr aux_list = CORBA::NVList::_nil();
   CORBA::NVList_ptr aux_list = 0;
   //CORBA::NamedValue_ptr nam_val = CORBA::NamedValue::_nil();
   CORBA::NamedValue_ptr nam_val = 0;

   CORBA::ULong aux_list_size = 0;
    
   const char* str_val = 0;
    
   for (CORBA::ULong i=0; i<contexts_count; i++) {
    
     ctx->get_values("",0,contexts->item(i),aux_list);
     aux_list_size = aux_list->count();
     for (CORBA::ULong j=0; j< aux_list_size; j++) {
       nam_val = aux_list->item(j);
       nv_context.push_back(nam_val->name());
       *(nam_val->value()) >>= str_val;
       nv_context.push_back(str_val);				
       CORBA::release(nam_val);
     }
      
     CORBA::release(aux_list);

  }

  CORBA::ULong total_ctx_size = (CORBA::ULong) nv_context.size();

  output.write_ulong(total_ctx_size);

  
  for (CORBA::ULong j = 0; j < total_ctx_size; j++)			
    output.write_string(nv_context[j]);	
}


                                          
CORBA::Context_ptr CORBA::Context::_duplicate(CORBA::Context_ptr ctx)
{
  /*
  try {
   
    //jagd
    //TIDorb::core::ContextImpl* context = dynamic_cast<TIDorb::core::ContextImpl*> (ctx);
    TIDorb::core::ContextImpl* context = (TIDorb::core::ContextImpl*)ctx;
 

 
    if(context)
      context->_add_ref();
  
    return ctx;
    
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
  */
  return ctx;
}

CORBA::Context_ptr CORBA::Context::_nil()
{
  return 0;
}

void CORBA::release(CORBA::Context_ptr ctx)
{
  /*
  try {
    //jagd 
    //TIDorb::core::ContextImpl* context = dynamic_cast<TIDorb::core::ContextImpl*> (ctx);
    TIDorb::core::ContextImpl* context = (TIDorb::core::ContextImpl*)ctx;
  
    if(context)
      context->_remove_ref();

  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
  */
}

CORBA::Boolean CORBA::is_nil(CORBA::Context_ptr ctx)
{
  return !ctx;
}
