/////////////////////////////////////////////////////////////////////////
//
// File:        ORBServices.C
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

#include "TIDorb/core.h"

const char*  TIDorb::core::ORBServices::ROOT_POA_ID                   = "RootPOA";
const char*  TIDorb::core::ORBServices::POA_CURRENT_ID                = "POACurrent";
const char*  TIDorb::core::ORBServices::INTERFACE_REPOSITORY_ID       = "InterfaceRepository";
const char*  TIDorb::core::ORBServices::NAME_SERVICE_ID               = "NameService";
const char*  TIDorb::core::ORBServices::TRADING_SERVICE_ID            = "TradingService";
const char*  TIDorb::core::ORBServices::SECURITY_CURRENT_ID           = "SecurityCurrent";
const char*  TIDorb::core::ORBServices::TRANSACTION_CURRENT_ID        = "TransactionCurrent";
const char*  TIDorb::core::ORBServices::DYN_ANY_FACTORY_ID            = "DynAnyFactory";
const char*  TIDorb::core::ORBServices::ORB_POLICY_MANAGER_ID         = "ORBPolicyManager";
const char*  TIDorb::core::ORBServices::POLICY_CURRENT_ID             = "PolicyCurrent";
const char*  TIDorb::core::ORBServices::NOTIFICATION_SERVICE_ID       = "NotificationService";
const char*  TIDorb::core::ORBServices::TYPED_NOTIFICATION_SERVICE_ID = "TypedNotificationService";
const char*  TIDorb::core::ORBServices::CODEC_FACTORY_ID              = "CodecFactory";
const char*  TIDorb::core::ORBServices::PI_CURRENT_ID                 = "PICurrent";
const char*  TIDorb::core::ORBServices::COMPRESSION_MANAGER_ID        = "CompressionManager";


const TIDorb::core::ORBServices::ServiceCodeTable*  TIDorb::core::ORBServices::st_service_code_table =
TIDorb::core::ORBServices::int_service_code_table();


TIDorb::core::ORBServices::ServiceCodeTable* TIDorb::core::ORBServices::int_service_code_table()
{
  ServiceCodeTable* table = new  TIDorb::core::ORBServices::ServiceCodeTable();

  (*table)[ROOT_POA_ID]                   = ROOT_POA;
  (*table)[POA_CURRENT_ID]                = POA_CURRENT;
  (*table)[INTERFACE_REPOSITORY_ID]       = INTERFACE_REPOSITORY;
  (*table)[NAME_SERVICE_ID]               = NAME_SERVICE;
  (*table)[TRADING_SERVICE_ID]            = TRADING_SERVICE;
  (*table)[SECURITY_CURRENT_ID]           = SECURITY_CURRENT;
  (*table)[TRANSACTION_CURRENT_ID]        = TRANSACTION_CURRENT;
  (*table)[DYN_ANY_FACTORY_ID]            = DYN_ANY_FACTORY;
  (*table)[ORB_POLICY_MANAGER_ID]         = ORB_POLICY_MANAGER;
  (*table)[POLICY_CURRENT_ID]             = POLICY_CURRENT;
  (*table)[NOTIFICATION_SERVICE_ID]       = NOTIFICATION_SERVICE;
  (*table)[TYPED_NOTIFICATION_SERVICE_ID] = TYPED_NOTIFICATION_SERVICE;
  (*table)[CODEC_FACTORY_ID]              = CODEC_FACTORY;
  (*table)[PI_CURRENT_ID]                 = PI_CURRENT;
  (*table)[COMPRESSION_MANAGER_ID]        = COMPRESSION_MANAGER;

  return table;
}



TIDorb::core::ORBServices::ORBServices(TIDorb::core::TIDORB* orb)
  throw(TIDThr::SystemException)
 : m_orb(orb)

{
  //jagd 2
  /*
  m_service_table[ROOT_POA_ID] = CORBA::Object::_nil();
  m_service_table[POA_CURRENT_ID] = CORBA::Object::_nil();
  m_service_table[INTERFACE_REPOSITORY_ID] = CORBA::Object::_nil();
  m_service_table[NAME_SERVICE_ID] = CORBA::Object::_nil();
  m_service_table[TRADING_SERVICE_ID] = CORBA::Object::_nil();
  m_service_table[SECURITY_CURRENT_ID] = CORBA::Object::_nil();
  m_service_table[TRANSACTION_CURRENT_ID] = CORBA::Object::_nil();
  m_service_table[DYN_ANY_FACTORY_ID] = CORBA::Object::_nil();
  m_service_table[ORB_POLICY_MANAGER_ID] = CORBA::Object::_nil();
  m_service_table[POLICY_CURRENT_ID] = CORBA::Object::_nil();
  m_service_table[NOTIFICATION_SERVICE_ID] = CORBA::Object::_nil();
  m_service_table[TYPED_NOTIFICATION_SERVICE_ID] = CORBA::Object::_nil();
  m_service_table[CODEC_FACTORY_ID] = CORBA::Object::_nil();
  m_service_table[PI_CURRENT_ID] = CORBA::Object::_nil();
  */ 
  m_service_table[ROOT_POA_ID] = 0;
  m_service_table[POA_CURRENT_ID] =0; 
  m_service_table[INTERFACE_REPOSITORY_ID] = 0;
  m_service_table[NAME_SERVICE_ID] = 0;
  m_service_table[TRADING_SERVICE_ID] = 0;
  m_service_table[SECURITY_CURRENT_ID] = 0;
  m_service_table[TRANSACTION_CURRENT_ID] = 0;
  m_service_table[DYN_ANY_FACTORY_ID] = 0;
  m_service_table[ORB_POLICY_MANAGER_ID] = 0;
  m_service_table[POLICY_CURRENT_ID] = 0;
  m_service_table[NOTIFICATION_SERVICE_ID] = 0;
  m_service_table[TYPED_NOTIFICATION_SERVICE_ID] = 0;
  m_service_table[CODEC_FACTORY_ID] = 0;
  m_service_table[PI_CURRENT_ID] = 0;
  m_service_table[COMPRESSION_MANAGER_ID] = 0;
}

void TIDorb::core::ORBServices::destroy()
{
  ServiceTable::iterator p = m_service_table.begin();
  ServiceTable::iterator endp = m_service_table.end();

  while(p != endp)
  {
    CORBA::release((*p).second);
//MLG
    p++;
//EMLG        
  }
  
  ValueFactoryTable::iterator q = m_value_factory_table.begin();
  ValueFactoryTable::iterator endq = m_value_factory_table.end();
  while(q != endq)
  {
    ((*q).second)->_remove_ref();
//MLG
    q++;
//EMLG        
  }
  
  m_service_table.clear();
  m_value_factory_table.clear();  
}

CORBA::Object_ptr TIDorb::core::ORBServices::resolve_service(const char* object_name)
throw (CORBA::ORB::InvalidName, CORBA::SystemException)
{
  TIDThr::Synchronized synchro(*this);

  if(!object_name)
    throw CORBA::BAD_PARAM();

  ServiceTable::iterator p = m_service_table.find(object_name);

  if(p == m_service_table.end()) {

    // no service registered, try with default URL reference

    if(!m_orb->conf().default_initial_reference)
      throw  CORBA::ORB::InvalidName();

    string str (m_orb->conf().default_initial_reference);

    str += object_name;

    try {
      return m_orb->string_to_object(str.data());
    } catch (...) {
      throw  CORBA::ORB::InvalidName();
    }
  }


  CORBA::Object_ptr reference = (*p).second;

  if(CORBA::is_nil(reference)) { // Maybe an unitialized ORB Service

    return init_orb_service(object_name);

  } else {
    return CORBA::Object::_duplicate(reference);
  }

}

CORBA::Object_ptr TIDorb::core::ORBServices::init_orb_service(const char* object_name)
throw (CORBA::ORB::InvalidName, CORBA::SystemException)
{

  ServiceCodeTable::const_iterator p = st_service_code_table->find(object_name);

  if(p == st_service_code_table->end())
  throw  CORBA::INTERNAL();

  ORBServiceCode orb_service_val = (*p).second;

  //jagd 2
  //CORBA::Object_ptr obj = CORBA::Object::_nil();
  CORBA::Object_ptr obj = 0;

  switch (orb_service_val)
  {
    case ROOT_POA:
    {
      obj = m_orb->init_POA();
      m_service_table[ROOT_POA_ID] = obj;
      return CORBA::Object::_duplicate(obj);
    }
    case POA_CURRENT:
    {
      obj= m_orb->init_POACurrent();
      m_service_table[POA_CURRENT_ID] = obj;
      return CORBA::Object::_duplicate(obj);
    }
    case DYN_ANY_FACTORY:
    {
      obj = m_orb->init_DynAnyFactory();
      m_service_table[DYN_ANY_FACTORY_ID] = obj;
      return  CORBA::Object::_duplicate(obj);
    }
    case CODEC_FACTORY:
    {
      obj = m_orb->init_CodecFactory();
      m_service_table[CODEC_FACTORY_ID] = obj;
      return CORBA::Object::_duplicate(obj);
    }
    case ORB_POLICY_MANAGER:
    {
      obj = m_orb->init_PolicyManager();
      m_service_table[ORB_POLICY_MANAGER_ID] = obj;
      return CORBA::Object::_duplicate(obj);
    }
    case POLICY_CURRENT:
      {
      obj = m_orb->init_PolicyCurrent();
      m_service_table[POLICY_CURRENT_ID] = obj;
      return CORBA::Object::_duplicate(obj);
      }
    case COMPRESSION_MANAGER:
    {
      obj = m_orb->init_CompressionManager();
      m_service_table[COMPRESSION_MANAGER_ID] = obj;
      return CORBA::Object::_duplicate(obj);
    }
    case INTERFACE_REPOSITORY:
    case NAME_SERVICE:
    case TRADING_SERVICE:
    case SECURITY_CURRENT:
    case TRANSACTION_CURRENT:
    case NOTIFICATION_SERVICE:
    case TYPED_NOTIFICATION_SERVICE:
    case PI_CURRENT:
      throw CORBA::NO_IMPLEMENT("Service not available");
    default:
      break;
  }
  // Never reached
  return NULL;
}

void TIDorb::core::ORBServices::register_initial_reference
  (const char* object_name,
   CORBA::Object_ptr obj)
throw (CORBA::ORB::InvalidName, CORBA::SystemException)
{

  TIDThr::Synchronized synchro(*this);

  if(!object_name)
  throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);

  //jagd 2
  //  if(CORBA::is_nil(obj))
  if(!(obj))
  throw CORBA::BAD_PARAM(24, CORBA::COMPLETED_NO);


  ServiceCodeTable::const_iterator p = st_service_code_table->find(object_name);

  if(p != st_service_code_table->end()) // Is an ORB service?
  throw CORBA::ORB::InvalidName();

  ServiceTable::iterator q = m_service_table.find(object_name);

  if(q != m_service_table.end()) {
    CORBA::release((*q).second);
    m_service_table.erase(q);
  }

  m_service_table[object_name] = CORBA::Object::_duplicate(obj);
}



CORBA::ORB::ObjectIdList_ptr TIDorb::core::ORBServices::list_initial_services()
{
  TIDThr::Synchronized synchro(*this);

  CORBA::ULong size = m_service_table.size();

  CORBA::ORB::ObjectIdList_ptr list = new CORBA::ORB::ObjectIdList();

  list->length(size);

  ServiceTable::const_iterator p = m_service_table.begin();
  ServiceTable::const_iterator end = m_service_table.end();

  CORBA::ULong i = 0;

  while(p != end) {
    (*list)[i++] = CORBA::string_dup(((*(p++)).first).data());
  }

  return list;
}

void TIDorb::core::ORBServices::set_service(const char* object_name,
CORBA::Object_ptr obj)
{
  if(!object_name)
  throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);

   //jagd 2
   //if(CORBA::is_nil(obj))
  if(!(obj))
  throw CORBA::BAD_PARAM(24, CORBA::COMPLETED_NO);

  m_service_table[object_name] = obj;
}

void TIDorb::core::ORBServices::remove_service(const char* object_name)
{
  if(!object_name)
    throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);

  m_service_table[object_name] = 0;

  m_orb->remove_POA();
}

void  TIDorb::core::ORBServices::remove_initial_reference(const char* object_name)
throw (CORBA::ORB::InvalidName, CORBA::SystemException)
{
  TIDThr::Synchronized synchro(*this);

  if(!object_name)
  throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);

  ServiceCodeTable::const_iterator p = st_service_code_table->find(object_name);

  if(p != st_service_code_table->end()) // Is an ORB service?
  throw CORBA::ORB::InvalidName();

  ServiceTable::iterator q = m_service_table.find(object_name);

  if(q == m_service_table.end()) {
    throw CORBA::ORB::InvalidName();
  } else {
    CORBA::release((*q).second);
    m_service_table.erase(q);
  }
}

::CORBA::ValueFactory TIDorb::core::ORBServices::register_value_factory
    (const char* id,
     ::CORBA::ValueFactory factory)
{

  TIDThr::Synchronized synchro(*this);

  if(!id)
    throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);

  if(!factory)
    throw CORBA::BAD_PARAM(24, CORBA::COMPLETED_NO);

  CORBA::ValueFactory factory_to_return = NULL;
  ValueFactoryTable::iterator q = m_value_factory_table.find(id);

  if(q != m_value_factory_table.end()) {
    factory_to_return = (*q).second;
    ((*q).second)->_remove_ref();
    m_value_factory_table.erase(q);
  }
  
  factory->_add_ref();

  m_value_factory_table[id] = factory;
  
  return factory_to_return;
}
     
void TIDorb::core::ORBServices::unregister_value_factory
  (const char* id)
{
  TIDThr::Synchronized synchro(*this);

  if(!id)
    throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);

  ValueFactoryTable::iterator q = m_value_factory_table.find(id);

  if(q != m_value_factory_table.end()) {    
    ((*q).second)->_remove_ref();
    m_value_factory_table.erase(q);
  }

}
    
::CORBA::ValueFactory TIDorb::core::ORBServices::lookup_value_factory
    (const char* id)
{
  TIDThr::Synchronized synchro(*this);

  if(!id)
    throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
  
  ValueFactoryTable::iterator q = m_value_factory_table.find(id);

  if(q != m_value_factory_table.end()) {
    CORBA::ValueFactory factory = ((*q).second); 
    factory->_add_ref();
    return factory;    
  }

  return NULL;
}
  
  
