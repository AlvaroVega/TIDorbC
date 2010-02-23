/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/IndirectionNode.h
  
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

#ifndef __TIDorb_core_cdr_IndirectionNode_h_
#define __TIDorb_core_cdr_IndirectionNode_h_

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

/** The <code>Indirection</code> class represents a node in the indirections to
 * the typecodes marshaled in the actual Encapsulation. If the node is in an inner
 * Encapsulation then it will references to the father encapsulation node.
 *
 * <p>
 * Copyright 2002 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */
   
class IndirectionNode
{
  
  public:
    
    /** 
     * Constructor
     */
     
    IndirectionNode(); 
    
    /**
     * Destructor
     */ 
    ~IndirectionNode();       
       
    /** 
     * Looks for a TypeCode marshaled at this position 
     */       
    const CORBA::TypeCode_ptr lookup_typecode(CDR::AbsolutePosition position) const;
    const char*               lookup_string(CDR::AbsolutePosition position) const;
    const ValueTypeInfo&      lookup_value(CDR::AbsolutePosition position) const;
        
    void put(CDR::AbsolutePosition position, const CORBA::TypeCode_ptr tc);
    void put(CDR::AbsolutePosition position, const char* str);
    void put(CDR::AbsolutePosition position, const ValueTypeInfo& value);

    void duplicate(IndirectionNode* newIndirectionNode);
     
  protected:
  
    typedef map<CDR::AbsolutePosition, CORBA::TypeCode_var> TypeCodePositionTable;
    typedef map<CDR::AbsolutePosition,  CORBA::String_var> StringPositionTable;     
    typedef map<CDR::AbsolutePosition, ValueTypeInfo> ValuePositionTable;
    
    TypeCodePositionTable* get_typecode_table();
    StringPositionTable* get_string_table();
    ValuePositionTable* get_value_table(); 

    TypeCodePositionTable* m_typecode_table;
    StringPositionTable* m_string_table;
    ValuePositionTable* m_value_table;
        
};

}
}
}

#endif

