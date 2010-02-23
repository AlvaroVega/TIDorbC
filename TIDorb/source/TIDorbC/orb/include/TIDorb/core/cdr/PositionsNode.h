/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/PositionsNode.h
  
 Revisions:
 
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
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/cdr.h"

#ifndef __TIDorb_core_cdr_PositionsNode_h_
#define __TIDorb_core_cdr_PositionsNode_h_

#include "CORBA.h"
#include "TIDThr.h"

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#define __STD_ISTREAM__
#define __STD_OSTREAM__
#endif

#include <map>

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#undef __STD_ISTREAM__
#undef __STD_OSTREAM__
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
#endif

namespace TIDorb {
namespace core {
namespace cdr {

struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

class PositionsNode 
{

  public:
    // Problems under linux
    // typedef map<char*, CDR::AbsolutePosition, ltstr> TypeCodeTable;
    // typedef map<char*, CDR::AbsolutePosition, ltstr> IdTable;
    typedef map<string, CDR::AbsolutePosition> TypeCodeTable;
    typedef map<string, CDR::AbsolutePosition> IdTable;
    typedef map<CDR::Addr, CDR::AbsolutePosition> AddrTable;

  PositionsNode(CDR::AbsolutePosition position_from_root = 0);
  ~PositionsNode();

  /**
  * Saves a <code>id</code> position in the table.
  * @param id the Object Id of the marshaled object
  * @param position the object position in the buffer
  */

  void put(const CORBA::TypeCode_ptr tc, CDR::AbsolutePosition position);
  void put(const char* id, CDR::AbsolutePosition position);
  void put(const CORBA::ValueBase* val, CDR::AbsolutePosition position);

 /**
  * Looks up if an <code>Object</code> has been yet marshaled in a stream.
  * If the object has been marshaled in a father encapsulation, the it will remark it 
  * at the <code>lower_indirection</code>
  * @param id the Object Id of the marshaled Object.
  * @return 0 if not.
  */
  
  CDR::AbsolutePosition lookup (const CORBA::TypeCode_ptr tc) const;
  CDR::AbsolutePosition lookup (const char* id) const;
  CDR::AbsolutePosition lookup (const CORBA::ValueBase* val) const;
   
   // Allocators
   TypeCodeTable* get_types_table();   
   IdTable* get_id_table();
   AddrTable* get_value_table();
   
   void duplicate(PositionsNode* newPositionNode);

  protected:
    TypeCodeTable* m_types_table;
    IdTable* m_id_table;
    AddrTable* m_value_table; 
};

}
}
}

#endif
