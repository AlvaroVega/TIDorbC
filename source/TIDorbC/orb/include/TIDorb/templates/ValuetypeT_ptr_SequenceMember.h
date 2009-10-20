/*/////////////////////////////////////////////////////////////////////////
//
// File:        ValuetypeT_ptr.h
// Description: ValueTypeT_ptr template
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////*/

#ifndef _TIDORB_TEMPLATES_VALUETYPE_T_PTR_SEQUENCE_MEMBER_H_
#define _TIDORB_TEMPLATES_VALUETYPE_T_PTR_SEQUENCE_MEMBER_H_

namespace TIDorb {
namespace templates {

  // Tipo ValuetypeT_ptr_SequenceMember
  //
  // Este es un tipo auxiliar no accesible al usuario disenado para
  // permitir que el tipo devuelto por uno de los operadores de indexacion de
  // las secuencias tenga la misma semantica que los tipos de los miembros
  // de las estructuras o elementos de arrays cuando estos han sido definidos
  // en IDL como valuetypes
  //

  template<class ValuetypeT> class ValuetypeT_ptr_SequenceMember
  {
    public:

    ValuetypeT_ptr_SequenceMember(CORBA::Boolean* rel) : release(rel){};

    ~ValuetypeT_ptr_SequenceMember() {};

    ValuetypeT* operator=(ValuetypeT* chp)
    {
      if (*release) {
        if (*ptr_str != 0) {
            (*ptr_str)->_remove_ref();
        }
      }
      *ptr_str = chp;   // se consume
      return *ptr_str;
    }
    
    ValuetypeT* operator=(const ValuetypeT_var<ValuetypeT>& s) 
    {
      if (*release) {
        if (*ptr_str != 0) {
            (*ptr_str)->_remove_ref();
        }
      }
      // copy (automatic cast)
      *ptr_str = ValuetypeT::_duplicate(s);
      return *ptr_str;
    }

    operator ValuetypeT*&() {return *ptr_str;}

    operator ValuetypeT*() const {return *ptr_str;}

    void asignar_ptr(ValuetypeT** p) {ptr_str= p;}

    private:
      ValuetypeT** ptr_str;
      CORBA::Boolean* release;
  };


} // end of namespace templates
} // end of namespace TIDorb

#endif
