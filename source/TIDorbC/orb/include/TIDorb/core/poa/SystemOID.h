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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
