/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        TypeCodeCache.h
// Description: 
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core/typecode.h"

#ifndef _TypeCodeCache_H_
#define _TypeCodeCache_H_

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#include "CORBA.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace typecode {

struct m_cache
{
  char* m_repid;
  CORBA::TypeCode_ptr m_typecode;
};
	
class TypeCodeCache
{
private:
  TIDThr::RecursiveMutex recursive_mutex;
  int m_max, m_index;
  
protected:
  m_cache** m_table;
  
public:
  TypeCodeCache(int max);
  ~TypeCodeCache();
  CORBA::TypeCode_ptr find(CORBA::RepositoryId rep_id);
  /**
   * Tries to insert one TypeCode in the TypeCodeCache.
   * 
   * @param type
   * @return if there is also a TypeCode with the same id, returns the
   *         TypeCode cached otherwise, the same Typecode put. *
   */    
  CORBA::TypeCode_ptr put(CORBA::TypeCode_ptr type) throw (CORBA::INTERNAL);
  void destroy();
};

}
}
}
#endif

