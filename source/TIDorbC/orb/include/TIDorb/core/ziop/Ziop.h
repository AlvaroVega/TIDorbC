/////////////////////////////////////////////////////////////////////////
//
// File:        ZIOP.h
// Description:
//              
// Rel:         01.00
// Created:     April, 2008
//
// Revised:
//
// Copyright 2008 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/ziop.h"

#ifndef _TIDORB_CORE_ZIOP_ZIOP_H_
#define _TIDORB_CORE_ZIOP_ZIOP_H_

namespace TIDorb {
namespace core {
namespace ziop {

class Ziop {

public:


  /**
   * Check if CompressionEnablingPolicy is defined in both context 
   * @param context
   * @return bool
   */
  static CORBA::Boolean
  checkCompressionEnabled(const TIDorb::core::PolicyContext& context);

  /**
   * Check if CompressionEnablingPolicy is defined in both context and ior_context
   * @param context
   * @param context
   * @return bool
   */
  static CORBA::Boolean
  checkCompressionEnabled(const TIDorb::core::PolicyContext& context,
                          const TIDorb::core::PolicyContext& ior_context);


  /**
   * Get the CompressorId to be used by client and compatible with IOR ProfileZIOP
   * and defined policy_context
   * @param context
   * @param context
   * @return CompressorId
   */
  static Compression::CompressorIdLevel
  getClientCompressor(const TIDorb::core::PolicyContext& context,
                      const TIDorb::core::PolicyContext& ior_context,
                      bool assume_ziop_server);

  static Compression::CompressionLevel
  getCompressionLevel(Compression::CompressorId compressor_id,
                     const TIDorb::core::PolicyContext& context);


  static CORBA::ULong
  getLowValue(const TIDorb::core::PolicyContext& context);


  static ZIOP::CompressionMinRatioPolicyValue
  getMinRatio(const TIDorb::core::PolicyContext& context);


};

}// ziop
}// core
}// TIDorb
#endif
