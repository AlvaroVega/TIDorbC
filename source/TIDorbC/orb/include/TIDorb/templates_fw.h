///////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/templates_fw.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
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
///////////////////////////////////////////////////////////////////////////

#ifndef _TIDORB_templates_fw_H_
#define _TIDORB_templates_fw_H_ 

#include "TIDorb/config.h"


namespace TIDorb {
namespace templates {

  template<class T>  class InterfaceT_out;
  template<class InterfaceT> class InterfaceT_ptr_SequenceMember;
  template<class InterfaceT> class InterfaceT_var;
  template<class T> class RefCount_out;
  template<class T> class RefCount_var;
  template<typename T, typename ContentT> class SequenceT_out;
  template<typename T, typename ContentT > class SequenceT_var;
  template<typename T> class FixedSizeT_var;
  template<typename T> class VariableSizeT_out;
  template<typename T> class VariableSizeT_var;
  template<class T> class ValuetypeT_out;
  template<class ValuetypeT> class ValuetypeT_ptr_SequenceMember;
  template<class T> class ValuetypeT_var;

}
}


#endif
