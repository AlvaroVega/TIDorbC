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
