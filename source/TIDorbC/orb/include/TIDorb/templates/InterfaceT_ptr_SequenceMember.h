/*/////////////////////////////////////////////////////////////////////////
//
// File:        InterfaceT_ptr_SequenceMember.h
// Description: InterfaceT_ptr_SequenceMember template
//              
// Rel:         01.00
// Created:     November, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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

#ifndef _TIDORB_TEMPLATES_INTERFACE_T_PTR_SEQUENCE_MEMBER_H_
#define _TIDORB_TEMPLATES_INTERFACE_T_PTR_SEQUENCE_MEMBER_H_

namespace TIDorb {
namespace templates {

  // Tipo InterfaceT_ptr_SequenceMember
  //
  // Este es un tipo auxiliar no accesible al usuario disenado para
  // permitir que el tipo devuelto por uno de los operadores de indexacion de
  // las secuencias tenga la misma semantica que los tipos de los miembros
  // de las estructuras o elementos de arrays cuando estos han sido definidos
  // en IDL como interfaces
  //

  template<class InterfaceT> class InterfaceT_ptr_SequenceMember
  {
    public:

    InterfaceT_ptr_SequenceMember(CORBA::Boolean* rel) : release(rel){};

    ~InterfaceT_ptr_SequenceMember() {};

    InterfaceT* operator=(InterfaceT* chp)
    {
      if (*release)
      CORBA::release(*ptr_str);
      *ptr_str = chp;   // se consume
      return *ptr_str;
    }
    
    InterfaceT* operator=(const InterfaceT_var<InterfaceT>& s) 
    {
      if (*release)
        CORBA::release(*ptr_str);
  
      // copy (automatic cast)
      *ptr_str = InterfaceT::_duplicate(s);
  
      return *ptr_str;
    }

    operator InterfaceT*&() {return *ptr_str;}

    operator InterfaceT*() const {return *ptr_str;}

    void asignar_ptr(InterfaceT** p) {ptr_str= p;}

    private:
      InterfaceT** ptr_str;
      CORBA::Boolean* release;
  };


} // end of namespace templates
} // end of namespace TIDorb

#endif
