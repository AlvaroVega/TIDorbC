/////////////////////////////////////////////////////////////////////////
//
// File:        SSLComponent.C
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

#include "TIDorb/core/comm/ssliop.h"




TIDorb::core::comm::ssliop::SSLComponent::SSLComponent() 
  throw(TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_SSL_SEC_TRANS)
{
  _str = NULL;
}




TIDorb::core::comm::ssliop::SSLComponent::SSLComponent(SSLIOP::SSL ssl) 
  throw(TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_SSL_SEC_TRANS),
    _ssl(ssl)
{
  _str = NULL;
}




TIDorb::core::comm::ssliop::SSLComponent::SSLComponent(const SSLComponent& v) 
  throw(TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_SSL_SEC_TRANS),
    _ssl(v._ssl)
{
  _str = NULL;
}




TIDorb::core::comm::ssliop::SSLComponent::~SSLComponent() 
  throw(TIDThr::SystemException)
{
  CORBA::string_free(_str);
}


Security::AssociationOptions 
TIDorb::core::comm::ssliop::SSLComponent::getTargetSupports()
{
  return _ssl.target_supports;
}
 

Security::AssociationOptions 
TIDorb::core::comm::ssliop::SSLComponent::getTargetRequires()
{
  return _ssl.target_requires;
}


CORBA::UShort TIDorb::core::comm::ssliop::SSLComponent::getSSLPort()
{
  return _ssl.port;
}


const SSLIOP::SSL* TIDorb::core::comm::ssliop::SSLComponent::getSSL()
{
  return &_ssl;
}
  
void TIDorb::core::comm::ssliop::SSLComponent::write(TIDorb::core::cdr::CDROutputStream& out) const
{
  TIDThr::Synchronized synchro(*((SSLComponent*)this));

  if(!_component_data) {

    TIDorb::core::cdr::CDROutputStream* encapsulation = out.create_encapsulation();
                       
    SSLIOP::_SSLHelper::write(*encapsulation, _ssl);
   
    ((SSLComponent*) this)->_component_data = (TIDorb::core::cdr::CDRInputStream* )
                                              (encapsulation->create_input_stream());
    delete encapsulation;
  }
  
  TaggedComponent::write(out);

}



	
void TIDorb::core::comm::ssliop::SSLComponent::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
  CORBA::string_free(_str);
  _str = NULL;

  TIDorb::core::cdr::CDRInputStream* encapsulation = input.read_encapsulation();
  SSLIOP::_SSLHelper::read(*encapsulation, _ssl);

  // Another way to do the same:
  //  TaggedComponent::partial_read(input);
  //   _component_data->read_ushort(_ssl.target_supports);
  //   _component_data->read_ushort(_ssl.target_requires);
  //   _component_data->read_ushort(_ssl.port);
  //   _component_data->rewind();
  
  delete encapsulation;
}




const char* TIDorb::core::comm::ssliop::SSLComponent::toString() const
{
  if (!_str) {
    TIDorb::util::StringBuffer buffer;
    buffer << "SSLComponent { "; 
    buffer << "port " << _ssl.port << " | ";
    buffer << "target_supports " << _ssl.target_supports << " | ";
    buffer << "target_requires " << _ssl.target_requires;
    buffer << " } ";
    ((SSLComponent*) this)->_str = CORBA::string_dup(buffer.str().data());
  }
	
  return _str;
}
