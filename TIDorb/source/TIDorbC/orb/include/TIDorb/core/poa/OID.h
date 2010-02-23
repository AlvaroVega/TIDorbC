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
