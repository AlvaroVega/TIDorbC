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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

