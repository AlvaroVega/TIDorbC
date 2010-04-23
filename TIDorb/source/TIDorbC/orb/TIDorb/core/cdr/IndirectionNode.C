/////////////////////////////////////////////////////////////////////////
//
// File:        IndirectionNode.C
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

#include "TIDorb/core/cdr.h"

namespace TIDorb {
namespace core {
namespace cdr {


    
IndirectionNode::IndirectionNode()
: m_typecode_table(NULL), 
  m_string_table(NULL), 
  m_value_table(NULL)
{
}


IndirectionNode::~IndirectionNode()
{ 
  if (m_typecode_table != NULL)
    delete m_typecode_table; 

  if (m_string_table != NULL)
    delete m_string_table;
  
  if (m_value_table != NULL)
    delete m_value_table;
}

/**
* Search an indirected object in the buffer
*/

const CORBA::TypeCode_ptr 
    IndirectionNode::lookup_typecode (CDR::AbsolutePosition position) const
{
  if(!m_typecode_table) { 
    throw CORBA::MARSHAL();
  }
     
  TypeCodePositionTable::const_iterator p = m_typecode_table->find(position);
  
  if(p == m_typecode_table->end()) {    
    throw CORBA::MARSHAL();
  } else {
    return (const CORBA::TypeCode_ptr) (p->second);
  }
}

const char* 
    IndirectionNode::lookup_string (CDR::AbsolutePosition position) const
{
  if(!m_string_table)
    throw CORBA::MARSHAL();
   
  StringPositionTable::const_iterator p = m_string_table->find(position);
  if(p == m_string_table->end())
    throw CORBA::MARSHAL();
  else
    return p->second;
}

const ValueTypeInfo& 
    IndirectionNode::lookup_value (CDR::AbsolutePosition position) const
{
 
  if(!m_value_table)
    throw CORBA::MARSHAL();
   
  ValuePositionTable::const_iterator p = m_value_table->find(position);
  if(p == m_value_table->end())
    throw CORBA::MARSHAL();
  else
    return (const ValueTypeInfo&) (p->second);
}

void IndirectionNode::put(CDR::AbsolutePosition position, 
                          const CORBA::TypeCode_ptr tc)
{
  (*get_typecode_table())[position] = CORBA::TypeCode::_duplicate(tc);
}

void IndirectionNode::put(CDR::AbsolutePosition position, const char* str)
{
  (*get_string_table())[position] = CORBA::string_dup(str);
}

void IndirectionNode::put(CDR::AbsolutePosition position, 
                         const ValueTypeInfo& info)
{
  (*get_value_table())[position] = info;
}

IndirectionNode::TypeCodePositionTable* IndirectionNode::get_typecode_table()
{
 
  if(m_typecode_table == 0) {
    m_typecode_table = new TypeCodePositionTable();
  }
    
  return m_typecode_table;
}
 
IndirectionNode::StringPositionTable* IndirectionNode::get_string_table()
{
  
  if(m_string_table == 0) {
    m_string_table = new StringPositionTable();
  }
    
  return m_string_table;
}

IndirectionNode::ValuePositionTable* IndirectionNode::get_value_table() 
{
  
  if(m_value_table == 0) {
    m_value_table = new ValuePositionTable();
  }
    
  return m_value_table;
}

void IndirectionNode::duplicate(IndirectionNode* newIndirectionNode)
{

  // Duplicate typecode table
  if (this->m_typecode_table != NULL){ 
    newIndirectionNode->m_typecode_table = new TypeCodePositionTable();
    TypeCodePositionTable::iterator it  = m_typecode_table->begin();
    TypeCodePositionTable::iterator end = m_typecode_table->end();
    while(it != end){
      (*newIndirectionNode->get_typecode_table())[it->first] =  
        CORBA::TypeCode::_duplicate(it->second);
      it++;
    }   
  }
  else
    newIndirectionNode->m_typecode_table = NULL;


  // Duplicate string position table
  if (this->m_string_table != NULL){
    newIndirectionNode->m_string_table = new StringPositionTable();
    StringPositionTable::iterator it  = m_string_table->begin();
    StringPositionTable::iterator end = m_string_table->end();
    while(it != end){
      (*newIndirectionNode->get_string_table())[it->first] =  
        CORBA::string_dup(it->second);
      it++;
    }   
  }
  else
    newIndirectionNode->m_string_table = NULL;


  // Duplicate value position table
  if (this->m_value_table != NULL){
    newIndirectionNode->m_value_table = new ValuePositionTable();
    ValuePositionTable::iterator it  = m_value_table->begin();
    ValuePositionTable::iterator end = m_value_table->end();
    while(it != end){
      (*newIndirectionNode->get_value_table())[it->first] = it->second;
      it++;
    }   
  }
  else
    newIndirectionNode->m_value_table = NULL;


}


} // cdr
} // core
} // TIDorb

