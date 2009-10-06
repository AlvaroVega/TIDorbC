//////////////////////////////////////////////////////////////////////////////////
//
// File:        CodecFactoryImpl.h
// Description: IOP::CodecFactory implementation.
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

#ifndef _TIDORB_CORE_CODEC_FACTORY_IMPL_H_
#define _TIDORB_CORE_CODEC_FACTORY_IMPL_H_

#include "CORBA.h"
#include "IOP.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {

class CodecFactoryImpl: public virtual IOP::CodecFactory,
                        public TIDThr::RefCounter, 
                        public virtual CORBA::LocalObject
                        
{
  public:
    CodecFactoryImpl(TIDorb::core::TIDORB* orb) throw (TIDThr::SystemException);
    ~CodecFactoryImpl() throw (TIDThr::SystemException){}
    
    virtual IOP::Codec_ptr create_codec(const ::IOP::Encoding& enc)
    throw (IOP::CodecFactory::UnknownEncoding);
        
    CORBA::Boolean _is_a(const char* repositoryIdentifier);
    
  private:
    TIDorb::core::TIDORB* m_orb;
};

}
}

#endif
