/////////////////////////////////////////////////////////////////////////
//
// File:        ServerRequestImpl.C
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

#include "TIDorb/core.h"
#include "TIDorb/core/util/exceptions.h"


using namespace TIDorb::core::ServerRequestStatus;




TIDorb::core::ServerRequestImpl::ServerRequestImpl
  (const TIDorb::core::comm::iiop::Version& ver,
   TIDorb::core::comm::iiop::RequestId id,
   char* operation,
   TIDorb::core::cdr::CDRInputStream* cdr_parameters,
   TIDorb::core::comm::iiop::TargetAddress* target,
   Compression::CompressorIdLevel compressor)
   : m_completion_status(NO_ARGUMENTS_FIXED),
     m_return_status(NO_EXCEPTION),
     m_request_id(id),
     m_with_response(false),
     m_operation_name(operation),
     m_target(target),
     m_version(ver),
     m_context(0),
     m_server_parameters(0),
     m_system_exception(0),
     //jagd 2
     //m_forward_obj(CORBA::Object::_nil())
     m_forward_obj(0),
     m_marshaled_parameters(cdr_parameters),
     m_compressor(compressor)
{
}



TIDorb::core::ServerRequestImpl::ServerRequestImpl
    (const TIDorb::core::ServerRequestImpl& other)
   : m_completion_status(NO_ARGUMENTS_FIXED),
     m_return_status(NO_EXCEPTION),
     m_request_id(other.m_request_id),
     m_with_response(other.m_with_response),
     m_version(other.m_version),
     m_context(0),
     m_server_parameters(0),
     m_system_exception(0),
     //jagd 2
     //m_forward_obj(CORBA::Object::_nil())
     m_forward_obj(0),
     m_compressor(Compression::CompressorIdLevel(0,0))
{
  m_operation_name = CORBA::string_dup(other.m_operation_name);
  m_marshaled_parameters = other.m_marshaled_parameters->copy();
  m_target = new TIDorb::core::comm::iiop::TargetAddress(*(other.m_target)); 
}


TIDorb::core::ServerRequestImpl::~ServerRequestImpl()
{
  CORBA::string_free(m_operation_name);
  //jagd 2 
  //CORBA::release(m_context);
  delete (m_context);
  CORBA::release(m_server_parameters);
  CORBA::release(m_forward_obj);
  delete m_marshaled_parameters;
  delete m_target;
  delete m_system_exception;
}




const char* TIDorb::core::ServerRequestImpl::operation() const
{
  return m_operation_name;
}




CORBA::Context_ptr TIDorb::core::ServerRequestImpl::ctx()
{
  // Check if called before "arguments()" (completion_state NO_ARGUMENTS_FIXED)
  // or after "set_result()" or "set_exception()" (completion_state RESULT_FIXED)
  // TODO: check if ctx() is called again!
  if(m_completion_status != ARGUMENTS_FIXED)
    throw CORBA::BAD_INV_ORDER(8, CORBA::COMPLETED_NO);

  //jagd 2
  //return CORBA::Context::_duplicate(m_context);
  return (m_context);
}




void TIDorb::core::ServerRequestImpl::arguments(CORBA::NVList_ptr& parameters)
{
  // Check if called again (completion_state ARGUMENTS_FIXED)
  // of after "set_exception()" (completion_state RESULT_FIXED)
  if(m_completion_status != NO_ARGUMENTS_FIXED) {
    throw CORBA::BAD_INV_ORDER(7, CORBA::COMPLETED_NO);
  }

  m_completion_status = ARGUMENTS_FIXED;

  //m_server_parameters = dynamic_cast <NVListImpl*>(CORBA::NVList::_duplicate(parameters));
  m_server_parameters = (NVListImpl*)CORBA::NVList::_duplicate(parameters)->_impl();

  NVListImpl::read_params_in(m_server_parameters, *m_marshaled_parameters);

  try {
    //jagd 2
    //CORBA::Context_ptr ctx = CORBA::Context::_nil();
    CORBA::Context_ptr ctx = 0;
    //XXXXXXXXMAL m_marshaled_parameters->read_Context(ctx);
    //jagd
    //m_context = dynamic_cast<ContextImpl*> (ctx);
    m_context = (ContextImpl*) (ctx);
  } catch (...) {
  }
}




void TIDorb::core::ServerRequestImpl::set_result(const CORBA::Any& result)
{
  // Check if called before "arguments()" (completion_state NO_ARGUMENTS_FIXED)
  // or after "set_result()" or "set_exception()" (completion_state RESULT_FIXED)
  if(m_completion_status != ARGUMENTS_FIXED)
    throw CORBA::BAD_INV_ORDER(8, CORBA::COMPLETED_NO);

  m_completion_status = RESULT_FIXED;
  m_return_status = NO_EXCEPTION;
  m_result = result;
}




void TIDorb::core::ServerRequestImpl::set_exception(const CORBA::Any& except)
{
  CORBA::TypeCode_var type = except.type();
  if (type->kind()!= CORBA::tk_except)
    throw CORBA::BAD_PARAM(21, CORBA::COMPLETED_NO); // Any value must be a exception

  const CORBA::SystemException* system_ex = NULL;

  if(except >>= system_ex) { // is a system exception?
    set_system_exception(*system_ex);
  } else {
    m_completion_status = RESULT_FIXED;
    m_return_status = USER_EXCEPTION;
    m_exception = except;
  }
}




// TIDorb operations


void TIDorb::core::ServerRequestImpl::set_system_exception(const CORBA::SystemException& except)
{
  m_completion_status = RESULT_FIXED;
  m_return_status = SYSTEM_EXCEPTION;

  delete m_system_exception;
  m_system_exception = TIDorb::core::util::exceptions::SystemExceptionEncoder::copy(except);
}




void TIDorb::core::ServerRequestImpl::set_forward(CORBA::Object_ptr obj)
{
  m_completion_status = RESULT_FIXED;
  m_return_status = LOCATION_FORWARD;

  m_forward_obj = CORBA::Object::_duplicate(obj);
}




const TIDorb::core::comm::iiop::TargetAddress*
  TIDorb::core::ServerRequestImpl::get_target() const
{
  return m_target;
}




TIDorb::core::comm::iiop::RequestId TIDorb::core::ServerRequestImpl::get_id() const
{
  return m_request_id;
}




void TIDorb::core::ServerRequestImpl::set_id(TIDorb::core::comm::iiop::RequestId id)
{
  m_request_id = id;
}




void TIDorb::core::ServerRequestImpl::with_response(bool value)
{
  m_with_response = value;
}




bool TIDorb::core::ServerRequestImpl::with_response() const
{
  return m_with_response;
}




TIDorb::core::ServerRequestStatus::ReturnStatus TIDorb::core::ServerRequestImpl::get_return_status() const
{
  return m_return_status;
}




const CORBA::Any& TIDorb::core::ServerRequestImpl::get_result() const
{
  return m_result;
}




const CORBA::Any& TIDorb::core::ServerRequestImpl::get_exception() const
{
  return m_exception;
}




const CORBA::SystemException& TIDorb::core::ServerRequestImpl::get_system_exception() const
{
  return *m_system_exception;
}




CORBA::NVList_ptr TIDorb::core::ServerRequestImpl::get_parameters() const
{
  return CORBA::NVList::_duplicate(m_server_parameters);
}




CORBA::Object_ptr TIDorb::core::ServerRequestImpl::get_forward() const
{
  return CORBA::Object::_duplicate(m_forward_obj);
}




const TIDorb::core::comm::iiop::Version& TIDorb::core::ServerRequestImpl::getVersion() const
{
  return m_version;
}

Compression::CompressorIdLevel
TIDorb::core::ServerRequestImpl::get_compressor() const
{
  return m_compressor;
}

void TIDorb::core::ServerRequestImpl::set_compressor(
                               Compression::CompressorIdLevel compressor)
{
  m_compressor = compressor;
}
