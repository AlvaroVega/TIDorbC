/////////////////////////////////////////////////////////////////////////
//
// File:        PositionsNode.C
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

TIDorb::core::cdr::PositionsNode::PositionsNode
  (TIDorb::core::cdr::CDR::AbsolutePosition position_from_root)
 : m_types_table(0),
   m_id_table(0),
   m_value_table(0)
{}

TIDorb::core::cdr::PositionsNode::~PositionsNode()
{
  if (m_types_table != NULL)
    delete m_types_table;

  if (m_id_table != NULL)
    delete m_id_table;

  if (m_value_table != NULL)
    delete m_value_table;

}



TIDorb::core::cdr::CDR::AbsolutePosition
 TIDorb::core::cdr::PositionsNode::lookup (const CORBA::TypeCode_ptr tc) const
{
    if(!m_types_table) {
      return 0;
    } 
    try {
      const char* id = tc->id();

      TypeCodeTable::iterator p = m_types_table->find(id);
      if(p == m_types_table->end())
        return 0;
      else
        return (*p).second; 

    }catch (CORBA::TypeCode::BadKind& bk) {
    	return 0;
    }	
}

TIDorb::core::cdr::CDR::AbsolutePosition
 TIDorb::core::cdr::PositionsNode::lookup (const char* id) const
{
    if(!m_id_table) {
      return 0;
    } 
    IdTable::iterator p = m_id_table->find(id);
    if(p == m_id_table->end())
      return 0;
    else
      return (*p).second; 
}

TIDorb::core::cdr::CDR::AbsolutePosition
  TIDorb::core::cdr::PositionsNode::lookup (const CORBA::ValueBase* val) const
{
    if(!m_value_table) {
      return 0;
    }
    
    AddrTable::iterator p = m_value_table->find((CDR::Addr) val);
    if(p == m_value_table->end())
      return 0;
    else
      return (*p).second; 
 
}

void TIDorb::core::cdr::PositionsNode::put
  (const CORBA::TypeCode_ptr tc, 
   TIDorb::core::cdr::CDR::AbsolutePosition position)
{	

   try {
      const char* id = tc->id();
      (*get_types_table())[id] = position;

   } catch (CORBA::TypeCode::BadKind& bk) {
   }
}

void TIDorb::core::cdr::PositionsNode::put
  (const char* id, 
   TIDorb::core::cdr::CDR::AbsolutePosition position)
{	
  (*get_id_table())[id] = position;
}

void TIDorb::core::cdr::PositionsNode::put
  (const CORBA::ValueBase* val, 
   TIDorb::core::cdr::CDR::AbsolutePosition position)
{	
  (*get_value_table())[(CDR::Addr) val] = position;
}

// Allocators
TIDorb::core::cdr::PositionsNode::TypeCodeTable* 
  TIDorb::core::cdr::PositionsNode::get_types_table()
{
  if(!m_types_table) {
    m_types_table = new TypeCodeTable();
  }
  
  return m_types_table;
}
   
TIDorb::core::cdr::PositionsNode::IdTable* 
  TIDorb::core::cdr::PositionsNode::get_id_table()
{
  if(!m_id_table) {
    m_id_table = new IdTable();
  }
  
  return m_id_table;
}
TIDorb::core::cdr::PositionsNode::AddrTable* 
  TIDorb::core::cdr::PositionsNode::get_value_table()
{
  if(!m_value_table) {
    m_value_table = new AddrTable();
  }
  
  return m_value_table;
}

void TIDorb::core::cdr::PositionsNode::duplicate(
                                 TIDorb::core::cdr::PositionsNode* newPositionsNode)
{
  // Duplicate types table
  if (this->m_types_table != NULL){
    newPositionsNode->m_types_table = new TypeCodeTable();
    TypeCodeTable::iterator it  = m_types_table->begin();
    TypeCodeTable::iterator end = m_types_table->end();
    while(it != end){
      (*newPositionsNode->get_types_table())[it->first] =  
        it->second;
      it++;
    }
  }
  else
    newPositionsNode->m_types_table = NULL;

  // Duplicate id table
  if (this->m_id_table != NULL){
    newPositionsNode->m_id_table = new IdTable();
    IdTable::iterator it  = m_id_table->begin();
    IdTable::iterator end = m_id_table->end();
    while(it != end){
      (*newPositionsNode->get_id_table())[it->first] =  
        it->second;
      it++;
    }
  }
  else
    newPositionsNode->m_id_table = NULL;

  // Duplicate value table
  if (this->m_value_table != NULL){
    newPositionsNode->m_value_table = new AddrTable();
    AddrTable::iterator it  = m_value_table->begin();
    AddrTable::iterator end = m_value_table->end();
    while(it != end){
      (*newPositionsNode->get_value_table())[it->first] =  
        it->second;
      it++;
    }
  }
  else
    newPositionsNode->m_value_table = NULL;

}
