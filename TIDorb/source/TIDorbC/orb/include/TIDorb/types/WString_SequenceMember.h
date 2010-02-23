/*/////////////////////////////////////////////////////////////////////////
//
// File:        WString_SequenceMember.h
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
/////////////////////////////////////////////////////////////////////////*/

#ifndef _TIDORB_TYPES_WSTRING_SEQUENCE_MEMBER_H_
#define _TIDORB_TYPES_WSTRING_SEQUENCE_MEMBER_H_

namespace TIDorb {
namespace types {

  // Tipo WString_SequenceMember
  //
  // Este es un tipo auxiliar no accesible al usuario disenado para
  // permitir que el tipo devuelto por uno de los operadores de indexacion de
  // las secuencias tenga la misma semantica que los tipos de los miembros 
  // de las estructuras o elementos de arrays cuando estos han sido definidos
  // en IDL como de tipo WString
  //
  
  class WString_SequenceMember 
  {
    public:

    WString_SequenceMember(CORBA::Boolean* rel) : release(rel){};
    ~WString_SequenceMember() {};

    CORBA::WChar* operator=(CORBA::WChar* chp);
    CORBA::WChar* operator=(const CORBA::WChar* chp);
    CORBA::WChar* operator=(const CORBA::WString_var& s);

    operator CORBA::WChar*&() const {return *ptr_str;}

    void asignar_ptr(CORBA::WChar** p) {ptr_str= p;}

  private:
    CORBA::WChar** ptr_str;
    CORBA::Boolean * release;
  };
  

} // end of namespace types
} // end of namespace TIDorb

#endif

