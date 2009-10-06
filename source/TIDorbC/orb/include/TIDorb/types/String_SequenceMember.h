/*/////////////////////////////////////////////////////////////////////////
//
// File:        String_SequenceMember.h
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

#ifndef _TIDORB_TYPES_STRING_SEQUENCE_MEMBER_H_
#define _TIDORB_TYPES_STRING_SEQUENCE_MEMBER_H_

namespace TIDorb {
namespace types {

  // Tipo String_SequenceMember
  //
  // Este es un tipo auxiliar no accesible al usuario disenado para
  // permitir que el tipo devuelto por uno de los operadores de indexacion de
  // las secuencias tenga la misma semantica que los tipos de los miembros 
  // de las estructuras o elementos de arrays cuando estos han sido definidos
  // en IDL como de tipo String
  //
  
  class String_SequenceMember 
  {
    public:

    String_SequenceMember(CORBA::Boolean* rel) : release(rel){};
    ~String_SequenceMember() {};

    char* operator=(char* chp);
    char* operator=(const char* chp);
    char* operator=(const CORBA::String_var& s);

    operator char*&() const {return *ptr_str;}

    void asignar_ptr(char** p) {ptr_str= p;}

  private:
    char** ptr_str;
    CORBA::Boolean * release;
  };
  

} // end of namespace types
} // end of namespace TIDorb

#endif

