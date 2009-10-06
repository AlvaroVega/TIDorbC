/////////////////////////////////////////////////////////////////////////
//
// File:        OID.h
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
 
#ifndef _TIDORB_CORE_POA_OID_H_
#define _TIDORB_CORE_POA_OID_H_

#include "PortableServer.h"
 
namespace TIDorb {
namespace core {
namespace poa {

class OID {

protected:

  PortableServer::ObjectId value; 
  
  CORBA::ULong hash_code;
  
  bool hash_created;
  
  char* str;
  
public:

  OID();

  OID(const PortableServer::ObjectId& val);
   
  OID(const OID& other);
        
  virtual ~OID();
  
  const PortableServer::ObjectId& get_object_id() const;
  PortableServer::ObjectId_ptr toByteArray() const;
  
  bool operator==(const OID& other) const;
  bool operator<(const OID& other) const;

  OID& operator=(const OID& other);
  
  CORBA::ULong hashCode() const;
  
  void fromString(const char* fromstr);
  
  const char* toString() const;
  
  void read(TIDorb::core::cdr::CDRInputStream& is);
  void write(TIDorb::core::cdr::CDROutputStream& os);

  protected:
      void reset();
};

} //poa
} //core
} //TIDorb
#endif
