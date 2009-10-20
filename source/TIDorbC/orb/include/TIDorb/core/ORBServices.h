//////////////////////////////////////////////////////////////////////////////////
//
// File:        ORBServices.h
// Description: ORBServices implementation.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_ORB_SERVICES_H_
#define _TIDORB_CORE_ORB_SERVICES_H_

#include "CORBA.h"
#include "TIDThr.h"

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#define __STD_ISTREAM__
#define __STD_OSTREAM__
#endif

#include <map>

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#undef __STD_ISTREAM__
#undef __STD_OSTREAM__
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
#endif

namespace TIDorb {
namespace core {

class ORBServices : public virtual TIDThr::RecursiveMutex
{
  public:

  static const char* ROOT_POA_ID;
  static const char* POA_CURRENT_ID;
  static const char* INTERFACE_REPOSITORY_ID;
  static const char* NAME_SERVICE_ID;
  static const char* TRADING_SERVICE_ID;
  static const char* SECURITY_CURRENT_ID; 
  static const char* TRANSACTION_CURRENT_ID;
  static const char* DYN_ANY_FACTORY_ID;
  static const char* ORB_POLICY_MANAGER_ID;
  static const char* POLICY_CURRENT_ID;
  static const char* NOTIFICATION_SERVICE_ID;
  static const char* TYPED_NOTIFICATION_SERVICE_ID;
  static const char* CODEC_FACTORY_ID; 
  static const char* PI_CURRENT_ID;
  static const char* COMPRESSION_MANAGER_ID;

  enum ORBServiceCode{ROOT_POA = 0, POA_CURRENT,INTERFACE_REPOSITORY, NAME_SERVICE, 
                      TRADING_SERVICE, SECURITY_CURRENT, TRANSACTION_CURRENT, DYN_ANY_FACTORY,
                      ORB_POLICY_MANAGER, POLICY_CURRENT, NOTIFICATION_SERVICE,
                      TYPED_NOTIFICATION_SERVICE, CODEC_FACTORY, PI_CURRENT,
                      COMPRESSION_MANAGER};
  
  typedef map<string, CORBA::Object_ptr> ServiceTable;
  typedef map<string, ORBServiceCode> ServiceCodeTable;
  typedef map<string, CORBA::ValueFactory> ValueFactoryTable;

  static const ServiceCodeTable* st_service_code_table;

  ORBServices(TIDorb::core::TIDORB* orb)
    throw (TIDThr::SystemException);

  ~ORBServices()
    throw (TIDThr::SystemException){};

  static ServiceCodeTable* int_service_code_table();
  
  CORBA::Object_ptr resolve_service(const char* object_name)
    throw (CORBA::ORB::InvalidName, CORBA::SystemException);

  void register_initial_reference(const char* object_name, CORBA::Object_ptr obj)
    throw (CORBA::ORB::InvalidName, CORBA::SystemException);

  CORBA::ORB::ObjectIdList_ptr list_initial_services();

  void set_service(const char* object_name, CORBA::Object_ptr obj);

  void remove_service(const char* object_name);

  void remove_initial_reference(const char* object_name)
    throw (CORBA::ORB::InvalidName, CORBA::SystemException);

  ::CORBA::ValueFactory register_value_factory(const char* id,
                                                 ::CORBA::ValueFactory factory);
     
  void unregister_value_factory(const char* id);
    
  ::CORBA::ValueFactory lookup_value_factory(const char* id);
  
  void destroy();
   
protected:

  CORBA::Object_ptr init_orb_service(const char* object_name)
    throw (CORBA::ORB::InvalidName, CORBA::SystemException);

  TIDorb::core::TIDORB* m_orb;
  ServiceTable m_service_table;
  ValueFactoryTable m_value_factory_table;

};

}
}

#endif
