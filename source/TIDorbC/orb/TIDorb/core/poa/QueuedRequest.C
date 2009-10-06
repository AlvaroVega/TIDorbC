/////////////////////////////////////////////////////////////////////////
//
// File:        QueuedRequest.C
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

#include "TIDorb/core/poa/QueuedRequest.h"

TIDorb::core::poa::QueuedRequest::QueuedRequest(TIDorb::core::poa::POAKey* poakey,
                                                TIDorb::core::poa::POAImpl* root_poa)
  : _poakey(poakey), 
    _current_child_poa_name_level(0), 
    mustDiscard(false),
    status(CORBA::COMPLETED_NO), 
    _current_poa(root_poa),   
    m_serial(0L),
    m_policy_context(NULL)
{
  _current_poa->_add_ref();
}




TIDorb::core::poa::QueuedRequest::~QueuedRequest()
{
  _current_poa->_remove_ref();
  // QueuedRequest is owner of m_policy_context because is duplicated in GIOPRequestMessage
  if (m_policy_context)
    delete m_policy_context;
}



CORBA::CompletionStatus TIDorb::core::poa::QueuedRequest::getStatus() const
{
  return status;
}



void TIDorb::core::poa::QueuedRequest::setStatus(CORBA::CompletionStatus value)
{
  status = value;
}



const char* TIDorb::core::poa::QueuedRequest::get_current_child_POA_name() const
{
  return _poakey->getPOA(_current_child_poa_name_level);
}




TIDorb::core::poa::POAImpl* TIDorb::core::poa::QueuedRequest::get_current_POA() const
{
  return _current_poa;
}




void TIDorb::core::poa::QueuedRequest::next_child_POA(TIDorb::core::poa::POAImpl* poa)
{
  _current_poa->_remove_ref();

  _current_poa = poa;

  _current_poa->_add_ref();

  _current_child_poa_name_level++;
}




TIDorb::core::poa::POAKey* TIDorb::core::poa::QueuedRequest::getPOAKey() const
{
  return _poakey;
}




const TIDorb::core::poa::OID& TIDorb::core::poa::QueuedRequest::getOID() const
{
  return _poakey->getOID();
}




CORBA::Boolean TIDorb::core::poa::QueuedRequest::isFinalPOA() const
{
  return _current_child_poa_name_level >= _poakey->numberOfPOAs();
}




void TIDorb::core::poa::QueuedRequest::setMustDiscard(CORBA::Boolean value)
{
  mustDiscard = value;
}




CORBA::Boolean TIDorb::core::poa::QueuedRequest::getMustDiscard() const
{
  return mustDiscard;
}



void TIDorb::core::poa::QueuedRequest::setSerial(CORBA::Long serial)
{
  m_serial = serial;
}

CORBA::Long TIDorb::core::poa::QueuedRequest::getSerial() const
{
  return m_serial;
}

void TIDorb::core::poa::QueuedRequest::setPolicyContext(TIDorb::core::PolicyContext* context)
{
  m_policy_context = context;
}
    
TIDorb::core::PolicyContext* TIDorb::core::poa::QueuedRequest::getPolicyContext() const
{
  return m_policy_context;
}


void TIDorb::core::poa::QueuedRequest::destroy()
{
  delete this;
}


// void* TIDorb::core::poa::QueuedRequest::_impl()
// {
// 	return this;
// }

// const char* TIDorb::core::poa::QueuedRequest::_typeid()
// {
// 	//return CORBA::string_dup("QueuedRequest");
// 	return "QueuedRequest";
// }
