/////////////////////////////////////////////////////////////////////////
//
// File:        ActiveObjectMap.h
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

#ifndef _TIDORB_CORE_POA_ActiveObjectMap_H_
#define _TIDORB_CORE_POA_ActiveObjectMap_H_


#include "TIDorb/core/util.h"

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#define __STD_ISTREAM__
#define __STD_OSTREAM__
#endif

#include <map>

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#undef __STD_ISTREAM__
#undef __STD_OSTREAM__
#endif

#include <stdexcept>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
#endif

/**
 * Active Object Map.
*/
namespace TIDorb {
namespace core {
namespace poa {

class ServantNotFound : public virtual exception {};

class ActiveObjectMap {
public:
  /**
   * Inner class for the Active Object Map entries.
   */
  class AOMElement: public virtual TIDThr::Monitor {
  public:
    CORBA::ULong num_users; // the first user is the POA where oid is created
    PortableServer::Servant _servant;
    bool destroying;
    
    AOMElement() throw (TIDThr::SystemException);	
    AOMElement(const AOMElement& other) throw (TIDThr::SystemException);
    AOMElement(PortableServer::Servant servant) throw (TIDThr::SystemException);
    
    ~AOMElement() throw (TIDThr::SystemException);
    
    AOMElement& operator=(const AOMElement& other); 
    AOMElement& operator=(PortableServer::Servant servant); 
    bool operator==(const AOMElement& other);
    bool operator==(const PortableServer::Servant servant);
    
  };
  
  typedef TIDThr::HandleT<AOMElement> AOMElement_ref;
  typedef map<OID, AOMElement_ref> AOMTable;
  
  AOMTable _table;
  
  ~ActiveObjectMap();
  
  /**
   * Adds a new oid to the Active Object Map.
   * @param oid Object Id.
   * @param servant Servant which corresponds to oid.
   * @param uniqueId True if UNIQUE_ID policy is present in this POA.
   * @exception ServantAlreadyActive If servant is already in the AOM.
   * @exception ObjectAlreadyActive  If uniqueId is true and oid is already in the AOM.
   */
  void put(const TIDorb::core::poa::OID& oid, PortableServer::Servant servant, bool uniqueId) 
    throw (PortableServer::POA::ServantAlreadyActive,PortableServer::POA::ObjectAlreadyActive);
  
  
  /**
   * Gets the servant associated to oid.
   * @param oid Object Id.
   * @return The servant.
   * @exception ObjectNotActive  If oid is not in the AOM.
   */
  PortableServer::Servant get(const TIDorb::core::poa::OID& oid)
    throw (PortableServer::POA::ObjectNotActive);

  /**
   * Gets the servant associated to oid if an oid is in the Active Object Map.
   * @param oid Object Id.
   * @return The servant if the oid is in the Active Object Map.
   */
  PortableServer::Servant get_if_Active(const TIDorb::core::poa::OID& oid);

  /**
   * Gets the servant associated to oid and removes it from Active Object Map.
   * @param oid Object Id.
   * @return The servant if the oid is in the Active Object Map.
   */
  PortableServer::Servant get_and_remove(const TIDorb::core::poa::OID& oid)
    throw (PortableServer::POA::ObjectNotActive);
  
  /**
   * Removes the servant associated to oid.
   * @param oid Object Id.
   * @return Retuns the servant removed (if any).
   * @exception ObjectNotActive  If oid is not in the AOM.
   */
  PortableServer::Servant remove(const TIDorb::core::poa::OID& oid)
    throw (PortableServer::POA::ObjectNotActive);
  
  /**
   * Tests if a servant is in the Active Object Map.
   * @return Retuns true if the servant is in the Active Object Map.
   */
  bool contains(PortableServer::Servant servant);
  
  /**
   * Tests if an oid is in the Active Object Map.
   * @return Retuns true if the oid is in the Active Object Map.
   */
  bool isActive(const TIDorb::core::poa::OID& oid);
  
  /**
   * Tests if an oid is in the Active Object Map and 
   * Adds a new user for this oid.
   * @return Retuns true if the oid is in the Active Object Map.
   */
  bool addUser_if_Active(const TIDorb::core::poa::OID& oid);
  
  /**
   * Gets the first oid associated to a servant.
   * @param The servant.
   * @return The Object Id.
   */
  const TIDorb::core::poa::OID& get(PortableServer::Servant servant) throw(ServantNotFound);
  
  /**
   * Adds a new user for this oid.
   * @param The Object Id.
   * @exception ObjectNotActive  If oid is not in the AOM.
   */
  void addUser(const TIDorb::core::poa::OID& oid)
    throw (PortableServer::POA::ObjectNotActive);
  
  /**
   * Removes a user for this oid.
   * @param The Object Id.
   * @return Returns false if there are still users, otherwise returns true.
   * @exception ObjectNotActive  If oid is not in the AOM.
   */
  bool removeUser(const TIDorb::core::poa::OID& oid)
    throw (PortableServer::POA::ObjectNotActive);

  /**
   * Removes a user for this oid if oid is in the Active Object Map.
   * @param The Object Id.
   * @return Returns false if there are still users or 
   * oid isn't in the Active Object Map, otherwise returns true.
   * @exception ObjectNotActive  If oid is not in the AOM.
   */
  bool removeUser_if_Active(const TIDorb::core::poa::OID& oid);
  
  /**
   * Marks an oid as "destroying".
   * @param The Object Id.
   */
  void destroy(const TIDorb::core::poa::OID& oid)
    throw (PortableServer::POA::ObjectNotActive);
  
  /**
   * Tests if an oid is "destroying".
   * @return The entry in the Active Object Map. It may be used for synchronization purposes.
   */
  AOMElement* isDestroying(const TIDorb::core::poa::OID& oid);
  
};
  
} //poa
} //core
} //TIDorb
#endif

