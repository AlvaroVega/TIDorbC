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
