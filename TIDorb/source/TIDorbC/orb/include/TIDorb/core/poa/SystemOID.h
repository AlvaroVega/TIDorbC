/////////////////////////////////////////////////////////////////////////
//
// File:        SystemOID.h
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

#include "TIDorb/core/poa.h"

#ifndef _TIDORB_CORE_POA_SystemOID_H_
#define _TIDORB_CORE_POA_SystemOID_H_

namespace TIDorb {
namespace core {
namespace poa {
 
class SystemOID : public TIDorb::core::poa::OID {
private: 
 
  CORBA::ULongLong _poa_id; //Marca de tiempo del POA que creo el SystemOID expresado en milisegundos
  CORBA::ULongLong _serial; //Comienza en cero
 

public:
  SystemOID();
  
  SystemOID(CORBA::ULongLong poa_id, CORBA::ULongLong serial);
 
  SystemOID(const SystemOID& val);
 
   TIDorb::core::poa::SystemOID* next();
 
  CORBA::ULongLong getPOAId();
  
  CORBA::ULongLong getSerial();
 
  static TIDorb::core::poa::SystemOID* fromOID(const TIDorb::core::poa::OID& oid);
 
  static TIDorb::core::poa::SystemOID* fromByteArray(const CORBA::OctetSeq& buffer);

  static void write_ulonglong(CORBA::ULongLong value, 
                              CORBA::OctetSeq& buffer,
                              CORBA::ULong offset);
 
  static CORBA::ULongLong read_ulonglong(const CORBA::OctetSeq& buffer, 
                                         CORBA::ULong offset);
   
};
} //poa
} //core
} //TIDorb
#endif
