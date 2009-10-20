/////////////////////////////////////////////////////////////////////////
//
// File:        POAKey.h
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
#ifndef _TIDORB_CORE_POA_POAKEY_H_
#define _TIDORB_CORE_POA_POAKEY_H_

#include "TIDThr.h"
#include "PortableServer.h"
#include <vector>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::vector;
#endif


namespace TIDorb {
namespace core {
namespace poa {


class POAKey;
typedef vector<POAKey*> POAKeyList;


class POAKey : public TIDThr::RefCounter {
private:
  /**
    * Sequence of poa names from the rootPOA to the final POA.
    */
      
    TIDorb::core::poa::POAPath _poas;
    
    /**
    * Rereference durability. If 0 the reference is persistent, otherwise the reference is
    * transient, and it lifespan is joined to its POA lifespan.
    */
    CORBA::ULongLong _poa_id;
  
    /**
    * Object Id.
    */
    TIDorb::core::poa::OID* _oid;
  
    char* key_name;
  
public:
    POAKey() throw (TIDThr::SystemException);
    
    POAKey(const POAKey& other) throw (TIDThr::SystemException);
    
    ~POAKey() throw (TIDThr::SystemException);
    
    POAKey(PortableServer::POA* poa, 
           CORBA::ULongLong poa_id,
           TIDorb::core::poa::OID* oid)
      throw (TIDThr::SystemException);
    
    POAKey(const TIDorb::core::poa::POAPath& poas, 
           CORBA::ULongLong poa_id,
           TIDorb::core::poa::OID* oid)
      throw (TIDThr::SystemException);
        
    const char* getPOA(CORBA::ULong poa_level) const
      throw (TIDThr::SystemException);
    
    CORBA::ULong numberOfPOAs() const;
    
    void setOID(TIDorb::core::poa::OID* oid);
  
    const TIDorb::core::poa::OID& getOID() const;
  
    CORBA::ULongLong getPOAId() const;
    
    void write(TIDorb::core::cdr::CDROutputStream& output);
    
    void read(TIDorb::core::cdr::CDRInputStream& input);
    
    bool samePOA(const TIDorb::core::poa::POAKey& other) const;
    
    bool samePOA(CORBA::ULongLong poa_id, const TIDorb::core::poa::POAPath& poas) const;
    
    bool operator==(const TIDorb::core::poa::POAKey& other) const;
    
    TIDorb::core::poa::POAKey& operator=(const TIDorb::core::poa::POAKey& other);
    
    TIDorb::core::poa::POAKey* clone();
    
    char* getPOAPath() const;
  
    const char* toString();
};
}// poa
}// core
}// TIDorb
#endif  
