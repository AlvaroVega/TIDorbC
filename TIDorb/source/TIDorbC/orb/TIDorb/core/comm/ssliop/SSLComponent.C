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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
