/////////////////////////////////////////////////////////////////////////
//
// File:        TypeCodeCache.C
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

#include "TIDorb/core/typecode/TypeCodeCache.h"

TIDorb::core::typecode::TypeCodeCache::TypeCodeCache(int max) 
  : m_max(max), m_index(0)
{
#if !defined(__linux__) && !defined(__sun)
  m_table = (m_cache**) new m_cache*[m_max];
#else
  m_table = new m_cache*[m_max];
#endif
}

TIDorb::core::typecode::TypeCodeCache::~TypeCodeCache()
{
  if (m_table) {

    for (int i = 0; i < m_index; i++) {
      if (m_table[i]) {
        if (m_table[i]->m_repid)
          CORBA::string_free(m_table[i]->m_repid);
        if (m_table[i]->m_typecode)
                CORBA::release(m_table[i]->m_typecode);
        delete m_table[i];
      }
    }
    delete[] m_table;
  }
}

CORBA::TypeCode_ptr
TIDorb::core::typecode::TypeCodeCache::find(CORBA::RepositoryId rep_id)
{
  TIDThr::Synchronized synchro(recursive_mutex);
   
  char *pointer = NULL;

  if (!rep_id)
    return NULL;
  
  int i = 0;

  for (i = 0; i < m_index; i++) {
    pointer = m_table[i]->m_repid;
    if (strcmp(pointer,rep_id) == 0)
      break;
    pointer = NULL;
  }
  
  if (pointer) {
    return  CORBA::TypeCode::_duplicate(m_table[i]->m_typecode);
  }
  else
    return NULL;
}

CORBA::TypeCode_ptr
TIDorb::core::typecode::TypeCodeCache::put(CORBA::TypeCode_ptr type)
  throw (CORBA::INTERNAL)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  CORBA::RepositoryId rep_id = NULL;
  
  try{
    rep_id = (char*) type->id();
  }
  catch (const CORBA::TypeCode::BadKind& badKind) {
    throw CORBA::INTERNAL("Only Complex Typecodes can be put in the cache");
  }
  
  if (rep_id == NULL)
    throw CORBA::INTERNAL("Cannot get RepositoryId");

  char *pointer = NULL;
  int i = 0;

  for (i = 0; i < m_index; i++) {
    pointer = m_table[i]->m_repid;
    if (strcmp(pointer,rep_id) == 0)
      break;
    pointer = NULL;
  } 

  if (pointer) {
    // Anybody use return pointer (duplicate generates mem leak) 
    //	return CORBA::TypeCode::_duplicate(m_table[i]->m_typecode);
    return m_table[i]->m_typecode; 
  }
  else {
    if (m_index == m_max) {
      m_cache** mtable = NULL;
      m_max *= 2;
      
#if !defined(__linux__) && !defined(__sun)
      mtable = (m_cache**) new m_cache*[m_max];
#else
      mtable = new m_cache*[m_max];
#endif
      memcpy(mtable, m_table, m_max/2*sizeof(m_cache*));
      delete[] m_table;
      m_table = mtable;
    }
    m_table[m_index] = new m_cache;
    m_table[m_index]->m_repid = CORBA::string_dup(rep_id);
    m_table[m_index++]->m_typecode = type; // Yet duplicate by caller
    // m_table[m_index++]->m_typecode = CORBA::TypeCode::_duplicate(type);
    
    // Anybody use return pointer (duplicate generates mem leak)
    //return CORBA::TypeCode::_duplicate(type); // is leak if no catch ??
    return type; 
  }
  
  return NULL;
}

void
TIDorb::core::typecode::TypeCodeCache::destroy()
{
  if (m_table) {
    for (int i = 0; i < m_index; i++) {
      if (m_table[i]) {
        if (m_table[i]->m_repid)
          CORBA::string_free(m_table[i]->m_repid);
        if (m_table[i]->m_typecode)
          CORBA::release(m_table[i]->m_typecode);
        delete m_table[i];
      }
        }
    delete[] m_table;
  }
  m_table = NULL;
}
