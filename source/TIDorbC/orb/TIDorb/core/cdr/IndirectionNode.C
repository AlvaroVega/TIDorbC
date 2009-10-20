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
    //PRA: return 0;
    throw CORBA::MARSHAL();
   
  StringPositionTable::const_iterator p = m_string_table->find(position);
  if(p == m_string_table->end())
    //PRA: return 0;
    throw CORBA::MARSHAL();
  else
    //PRA
    //return CORBA::string_dup((*p).second);
    //return CORBA::string_dup((*p).second.data());
    return p->second;
    //EPRA
}

const ValueTypeInfo& 
    IndirectionNode::lookup_value (CDR::AbsolutePosition position) const
{
 
  if(!m_value_table)
    //PRA: return 0;
    throw CORBA::MARSHAL();
   
  ValuePositionTable::const_iterator p = m_value_table->find(position);
  if(p == m_value_table->end())
    //PRA: return 0;
    throw CORBA::MARSHAL();
  else
    //PRA: return (ValueTypeInfo*) ((*p).second);
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

