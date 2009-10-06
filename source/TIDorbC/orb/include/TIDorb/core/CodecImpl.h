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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
