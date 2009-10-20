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
