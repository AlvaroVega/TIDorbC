/////////////////////////////////////////////////////////////////////////
//
// File:        String_SequenceMember.C
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
/////////////////////////////////////////////////////////////////////////

#include "CORBA.h"

  // Tipo String_SequenceMember
  //
  // Este tipo es un tipo auxiliar no accesible al usuario disenado para
  // permitir que el tipo devuelto por uno de los operadores de indexacion,
  // tenga la misma semantica que los tipos de los miembros de las estructuras
  // o elementos de arrays cuando estos han sido definidos en IDL como de tipo
  // String
  //

#include "TIDorb/types.h"

char* TIDorb::types::String_SequenceMember::operator= (char* chp) {
	if (*release) 
		CORBA::string_free(*ptr_str);
	*ptr_str = chp;	// se consume
	return *ptr_str;
}
    
char* TIDorb::types::String_SequenceMember::operator=(const char* chp) {
	if (*release)
		CORBA::string_free(*ptr_str);
	*ptr_str = CORBA::string_dup(chp);	// se copia
	return *ptr_str;
}
    
    
char* TIDorb::types::String_SequenceMember::operator=(const CORBA::String_var& s) {
	if (*release)
		CORBA::string_free(*ptr_str);
	*ptr_str = CORBA::string_dup(s);	// se copia (cast automatico)
	return *ptr_str;
}
