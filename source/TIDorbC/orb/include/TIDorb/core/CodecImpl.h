//////////////////////////////////////////////////////////////////////////////////
//
// File:        CodecImpl.h
// Description: IOP::Codec implementation.
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

#ifndef _TIDORB_CORE_CODEC_IMPL_H_
#define _TIDORB_CORE_CODEC_IMPL_H_

#include "CORBA.h"
#include "IOP.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {

class CodecImpl: public virtual IOP::Codec,
                 public TIDThr::RefCounter,
                 public virtual CORBA::LocalObject    
{
  public:

    CodecImpl(TIDorb::core::TIDORB* orb, const IOP::Encoding& enc)
      throw (TIDThr::SystemException);

    ~CodecImpl() throw(TIDThr::SystemException) {}

    CORBA::OctetSeq* encode(const CORBA::Any& data)
      throw (IOP::Codec::InvalidTypeForEncoding);


    CORBA::Any* decode(const ::CORBA::OctetSeq& data)
      throw (IOP::Codec::FormatMismatch);

    CORBA::OctetSeq* encode_value(const CORBA::Any& data)
      throw (IOP::Codec::InvalidTypeForEncoding);


    CORBA::Any* decode_value(const ::CORBA::OctetSeq& data,
                             CORBA::TypeCode_ptr tc)
      throw (IOP::Codec::FormatMismatch, IOP::Codec::TypeMismatch);

  private:
    TIDorb::core::TIDORB* m_orb;
    IOP::Encoding         m_enc;
};

}// core
}// TIDorb

#endif
