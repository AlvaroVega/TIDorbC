/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/ContextCDR.h
  
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

#ifndef __TIDorb_core_cdr_ContextCDR_h_
#define __TIDorb_core_cdr_ContextCDR_h_

#include "CORBA.h"
#include "TIDThr.h"



namespace TIDorb {
namespace core {
namespace cdr {

/**
 * Represents the contexts of a buffer marshaling across different encapsulations.
 * The context is defined by:
 * <ul>
 * <li> the byte-order
 * <li> the father context
 * </ul>
 * The context creates a list of encapsulations for navigate in and out of
 * encapsulations. The list represented by the context is a 
 * <p>
 * Copyright 2002 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @see TIDorb/core/cdr/AlignmentOffset
 *
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */


class ContextCDR {
  public:


    ContextCDR(); 
    
    ContextCDR(const ContextCDR* father, 
               CDR::AbsolutePosition position_in_father);
    
       
    ~ContextCDR() {};

  
    const ContextCDR* get_father() const
    {
      return m_father;
    }   
    
    CDR::AbsolutePosition get_position_from_root() const
    {
      return m_position_from_root;
    }
    
    bool in_an_encapsulation() const
    {
      return m_father != 0;
    }
    
    bool is_root_context() const
    {
      return m_father == 0;
    }

    bool has_external_indirections() const
    {
        return m_has_external_indirections;
    }
   
    /* 
      Indirection Resolution: lookup if typecode, string or value was marshaled in the given
                              absolute position. 
     */
      
    const CORBA::TypeCode_ptr lookup_typecode(CDR::AbsolutePosition position) const;
    const char*               lookup_string(CDR::AbsolutePosition position) const;
    const ValueTypeInfo&      lookup_value(CDR::AbsolutePosition position) const;
    
    //EPRA
    
    CDR::AbsolutePosition lookup(const CORBA::TypeCode_ptr tc) const;
    CDR::AbsolutePosition lookup(const char* id) const;
    CDR::AbsolutePosition lookup(const CORBA::ValueBase* val) const;
      
    /**
     * The Typecode is at this position
     */
    void put(CDR::AbsolutePosition position, const CORBA::TypeCode_ptr tc);
    /**
     * The string is at this position
     */
    void put(CDR::AbsolutePosition position, const char* str);
    
    /**
     * The value has been marshaled at this position
     */
    void put(CDR::AbsolutePosition position, const ValueTypeInfo& val_info);
    
    /**
     * The value has been demarshaled at this position
     */
    void put(CDR::AbsolutePosition position, 
             const CORBA::ValueBase* value);
       
    //EPRA       

    void duplicate(ContextCDR* newContextCDR);

  protected:
 
    /**
    * The father context. In the root context, the father will be <code>null</code>.
    */
  
    const ContextCDR* m_father;
  
    /**
     * Relative position if it is an encapsulation
     */
    CDR::AbsolutePosition m_position_from_root;
    
    /**
     * This context has an Typecode Indirection out of these encapsulaton
     */
    bool m_has_external_indirections;
    
  
    /**
    * The indirection node of the buffer. Here are all the typecodes that have been
    * readed and can be indirectioned.
    */
  
    IndirectionNode m_indirection_node;
  
    /**
    * The TypeCode Position Node of the buffer. Here are all the typecodes that have been
    * writed in the buffer and can be indirectioned.
    */
  
    PositionsNode m_positions_node;



 private:
    TIDThr::Mutex mutex;

};

}
}
}

#endif
