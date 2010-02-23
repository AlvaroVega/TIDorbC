/////////////////////////////////////////////////////////////////////////
//
// File:        LocalServerRequest.C
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
#include "TIDorb/core/util/exceptions.h"

#include <typeinfo>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif


using namespace TIDorb::core::ServerRequestStatus;




TIDorb::core::LocalServerRequest::LocalServerRequest(TIDorb::core::RequestImpl* req)
  : m_completion_status(NO_ARGUMENTS_FIXED),
    m_local_request(req),
    m_server_parameters(0),
    m_forward_obj(CORBA::Object::_nil())
{
//MLG
    m_local_request->_add_ref();
//EMLG
}




TIDorb::core::LocalServerRequest::~LocalServerRequest()
{
//MLG
  m_local_request->_remove_ref();
//ELMG
  CORBA::release(m_server_parameters);
  CORBA::release(m_forward_obj);
}




const char* TIDorb::core::LocalServerRequest::operation() const
{
  return m_local_request->operation();
}




CORBA::Context_ptr TIDorb::core::LocalServerRequest::ctx()
{
  // Check if called before "arguments()" (completion_state NO_ARGUMENTS_FIXED)
  // or after "set_result()" or "set_exception()" (completion_state RESULT_FIXED)
  // TODO: check if ctx() is called again!
  if(m_completion_status != ARGUMENTS_FIXED)
    throw CORBA::BAD_INV_ORDER(8, CORBA::COMPLETED_NO);

  return m_local_request->ctx();
}




void TIDorb::core::LocalServerRequest::arguments(CORBA::NVList_ptr& parameters)
{
  // Check if called again (completion_state ARGUMENTS_FIXED)
  // of after "set_exception()" (completion_state RESULT_FIXED)
  if(m_completion_status != NO_ARGUMENTS_FIXED)
    throw CORBA::BAD_INV_ORDER(7, CORBA::COMPLETED_NO);

  if(parameters == 0)
    throw CORBA::BAD_PARAM();//("Null NVList reference", 0, CompletionStatus.COMPLETED_NO);

  m_completion_status = ARGUMENTS_FIXED;

  //m_server_parameters = dynamic_cast<NVListImpl*> (CORBA::NVList::_duplicate(parameters));
  m_server_parameters = (NVListImpl*)CORBA::NVList::_duplicate(parameters)->_impl();

  // TODO: performance improvement: wrap the parameter anys and do not copy them
  //                                ONLY WITH NO ONEWAY REQUEST!!!!!

  bool wrap_anys = m_local_request->with_response();

  CORBA::NVList_var local_args = m_local_request->arguments();

  NVListImpl::assign_in_arguments(local_args, m_server_parameters, wrap_anys);
}




void TIDorb::core::LocalServerRequest::set_result(const CORBA::Any& result)
{
  // Check if called before "arguments()" (completion_state NO_ARGUMENTS_FIXED)
  // or after "set_result()" or "set_exception()" (completion_state RESULT_FIXED)
  if(m_completion_status != ARGUMENTS_FIXED)
    throw CORBA::BAD_INV_ORDER(8, CORBA::COMPLETED_NO);

  m_local_request->return_value() = result;
  m_completion_status = RESULT_FIXED;

/*
  try {
    AnyImpl& result_any = dynamic_cast <AnyImpl&> (result.delegate());
    AnyImpl& local_result_any = dynamic_cast <AnyImpl&>
      m_local_request->return_value().delegate()->_impl();
      //(m_local_request->return_value().delegate());

    m_completion_status = RESULT_FIXED;
    local_result_any.assign_value(result_any);

  } catch (const bad_cast& bc) {
    throw CORBA::INTERNAL();
  }
*/
}




void TIDorb::core::LocalServerRequest::set_exception(const CORBA::Any& except)
{
  CORBA::TypeCode_var type = except.type();
  if (type->kind()!= CORBA::tk_except)
    throw CORBA::BAD_PARAM(21, CORBA::COMPLETED_NO); // Any value must be a exception

  const CORBA::SystemException* system_ex = NULL;

  if(except >>= system_ex) { // is a system exception?
    set_system_exception(*system_ex);
  } else {
    m_completion_status = RESULT_FIXED;
    m_local_request->set_user_exception(except);
  }
}




// TIDorb operations




void TIDorb::core::LocalServerRequest::set_system_exception(const CORBA::SystemException& except)
{
  m_completion_status = RESULT_FIXED;
  m_local_request->set_system_exception(
    TIDorb::core::util::exceptions::SystemExceptionEncoder::copy(except));
}




void TIDorb::core::LocalServerRequest::set_forward(CORBA::Object_ptr obj)
{
  m_completion_status = RESULT_FIXED;
  m_forward_obj = CORBA::Object::_duplicate(obj);
}




bool TIDorb::core::LocalServerRequest::is_forwarded()
{
  //jagd 2
  //return !CORBA::is_nil(m_forward_obj);
  return (bool)(m_forward_obj);
}




CORBA::Object_ptr TIDorb::core::LocalServerRequest::get_forward()
{
  return CORBA::Object::_duplicate(m_forward_obj);
}




void TIDorb::core::LocalServerRequest::fix_out_arguments()
{
  if (m_completion_status == NO_ARGUMENTS_FIXED)
    throw CORBA::BAD_INV_ORDER(0, CORBA::COMPLETED_NO);

  // TODO: performance improvement: wrap the parameter anys and do not copy them
  //                                ONLY WITH NO ONEWAY REQUEST!!!!!

  CORBA::NVList_var args = m_local_request->arguments();
  if (m_server_parameters) {
    NVListImpl::assign_out_arguments(m_server_parameters, args, false);
  }
}
