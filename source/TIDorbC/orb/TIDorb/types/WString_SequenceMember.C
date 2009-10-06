/////////////////////////////////////////////////////////////////////////
//
// File:        WString_SequenceMember.C
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
/////////////////////////////////////////////////////////////////////////


// Tipo WString_SequenceMember
//
// Este tipo es un tipo auxiliar no accesible al usuario disenado para
// permitir que el tipo devuelto por uno de los operadores de indexacion,
// tenga la misma semantica que los tipos de los miembros de las estructuras
// o elementos de arrays cuando estos han sido definidos en IDL como de tipo
// WString
//

#include "CORBA.h"
#include "TIDorb/types.h"

CORBA::WChar* TIDorb::types::WString_SequenceMember::operator= (CORBA::WChar* chp) {
	if (*release) 
		CORBA::wstring_free(*ptr_str);
	*ptr_str = chp;	// se consume
	return *ptr_str;
}
    
CORBA::WChar* TIDorb::types::WString_SequenceMember::operator=(const CORBA::WChar* chp) {
	if (*release)
		CORBA::wstring_free(*ptr_str);
	*ptr_str = CORBA::wstring_dup(chp);	// se copia
	return *ptr_str;
}
    
    
CORBA::WChar* TIDorb::types::WString_SequenceMember::operator=(const CORBA::WString_var& s) {
	if (*release)
		CORBA::wstring_free(*ptr_str);
	*ptr_str = CORBA::wstring_dup(s);	// se copia (cast automatico)
	return *ptr_str;
}
