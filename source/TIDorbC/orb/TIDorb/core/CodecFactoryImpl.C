/////////////////////////////////////////////////////////////////////////
//
// File:        CodecFactoryImpl.C
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

TIDorb::core::CodecFactoryImpl::CodecFactoryImpl(TIDorb::core::TIDORB* orb)
  throw (TIDThr::SystemException)
  : m_orb(orb)
{
  this->TIDThr::RefCounter::_add_ref();
}

IOP::Codec_ptr TIDorb::core::CodecFactoryImpl::create_codec(const ::IOP::Encoding& enc)
  throw (IOP::CodecFactory::UnknownEncoding)
{
  if (enc.format != IOP::ENCODING_CDR_ENCAPS)
    throw IOP::CodecFactory::UnknownEncoding();
  
  if(enc.major_version != 1)
    throw IOP::CodecFactory::UnknownEncoding();
  
  switch (enc.minor_version) 
    {
    case 0:
      return new CodecImpl(m_orb , enc);
    case 1:
      return new CodecImpl(m_orb , enc);
    case 2:
      return new CodecImpl(m_orb , enc);
    
    default:
      throw IOP::CodecFactory::UnknownEncoding();
  }   
  
}

CORBA::Boolean  TIDorb::core::CodecFactoryImpl::_is_a(const char* repositoryIdentifier)
{	
  if(!repositoryIdentifier)
    throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO); //"Null string reference");
  
  if(!strcmp(repositoryIdentifier,"IDL:omg.org/IOP/CodecFactory:1.0"))
    return true;
  else 
    return ::CORBA::LocalObject::_is_a(repositoryIdentifier);
}	
