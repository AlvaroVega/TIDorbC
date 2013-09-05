/////////////////////////////////////////////////////////////////////////
//
// File:        ZIOP.C
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

#include "TIDorb/core/ziop/Ziop.h"


/**
 * Check if CompressionEnablingPolicy is defined in context
 * @param context
 * @return bool
 */
CORBA::Boolean
TIDorb::core::ziop::Ziop::checkCompressionEnabled(
                               const TIDorb::core::PolicyContext& context)
{
  CORBA::Policy_ptr policy = NULL;
  policy = context.getPolicy(ZIOP::COMPRESSION_ENABLING_POLICY_ID);

  if (policy != NULL) {

    ZIOP::CompressionEnablingPolicy_var policy_ctx =
      ZIOP::CompressionEnablingPolicy::_narrow(policy); 

    CORBA::release(policy);

    return (policy_ctx->compression_enabled());

  } else {
    return false;
  }

}

/**
 * Check if CompressionEnablingPolicy is defined in both context and ior_context
 * @param context
 * @param context
 * @return bool
 */
CORBA::Boolean
TIDorb::core::ziop::Ziop::checkCompressionEnabled(
                        const TIDorb::core::PolicyContext& context,
                        const TIDorb::core::PolicyContext& ior_context)
{
  CORBA::Policy_ptr policy = NULL;
  policy = context.getPolicy(ZIOP::COMPRESSION_ENABLING_POLICY_ID);

  if (policy != NULL) {

    ZIOP::CompressionEnablingPolicy_ptr policy_ctx =
      ZIOP::CompressionEnablingPolicy::_narrow(policy); 

    CORBA::release(policy);

    policy = ior_context.getPolicy(ZIOP::COMPRESSION_ENABLING_POLICY_ID);

    if (policy != NULL) {
      ZIOP::CompressionEnablingPolicy_ptr policy_ior =
        ZIOP::CompressionEnablingPolicy::_narrow(policy); 

      CORBA::release(policy);

      return (policy_ctx->compression_enabled() && 
              policy_ior->compression_enabled() );

    }
    
  }
  return false;

}

/**
 * Get the CompressorId to be used by client and compatible with IOR ProfileZIOP
 * and defined policy_context
 * @param context
 * @return
 */

// TODO: return Compression::CompressorIdLevel
Compression::CompressorIdLevel
TIDorb::core::ziop::Ziop::getClientCompressor(
                                            const TIDorb::core::PolicyContext& context,
                                            const TIDorb::core::PolicyContext& ior_context,
                                            bool assume_ziop_server)
{
  //
  // Return the low compressor ID supported between context and ior_context
  //

  Compression::CompressorIdLevel result(0, 0);

  CORBA::Policy_ptr policy = NULL;
  policy = context.getPolicy(ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID);
  
  if (policy != NULL) {
    ZIOP::CompressorIdLevelListPolicy_ptr policy_ctx =
      ZIOP::CompressorIdLevelListPolicy::_narrow(policy); 
    
    CORBA::release(policy);

    if (&ior_context != NULL) {
      policy = ior_context.getPolicy(ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID);

      if (policy != NULL) {
        ZIOP::CompressorIdLevelListPolicy_ptr policy_ior =
          ZIOP::CompressorIdLevelListPolicy::_narrow(policy); 
        
        CORBA::release(policy);
        
        Compression::CompressorIdLevelList_var compressors_ctx = 
          policy_ctx->compressor_ids();
        Compression::CompressorIdLevelList_var compressors_ior = 
          policy_ior->compressor_ids();
        
        // Compare both lists
        for (CORBA::ULong i = 0; i < compressors_ctx->length(); i++) {
          
          for (CORBA::ULong j = 0; j < compressors_ior->length(); j++) {
            
            if ( (*compressors_ctx)[i].compressor_id == 
                 (*compressors_ior)[j].compressor_id ){ 
              result.compressor_id     = (*compressors_ctx)[i].compressor_id;
              result.compression_level = (*compressors_ctx)[i].compression_level;
              return result;
            }
            
            
          }
        }
      }
      
    } else {
      if (assume_ziop_server) {
        Compression::CompressorIdLevelList_var compressors_ctx = 
          policy_ctx->compressor_ids();
        if (compressors_ctx->length() > 0) {
          result.compressor_id     = (*compressors_ctx)[0].compressor_id;
          result.compression_level = (*compressors_ctx)[0].compression_level;
        }
      }
    }
    
  } 
  return result;
  
}



Compression::CompressionLevel
TIDorb::core::ziop::Ziop::getCompressionLevel(Compression::CompressorId compressor_id,
                                              const TIDorb::core::PolicyContext& context)
{
  //
  // Return compressor level prefered for compressor_id following context 
  //
  CORBA::Policy_ptr policy = NULL;
  policy = context.getPolicy(ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID);

  if (policy != NULL) {
    ZIOP::CompressorIdLevelListPolicy_ptr policy_ctx =
      ZIOP::CompressorIdLevelListPolicy::_narrow(policy); 
    
    CORBA::release(policy);

    Compression::CompressorIdLevelList_var compressors_ctx = 
      policy_ctx->compressor_ids();

    // Compare both lists
    for (CORBA::ULong i = 0; i < compressors_ctx->length(); i++) {
      
      if ( (*compressors_ctx)[i].compressor_id == compressor_id ) { 
        return (*compressors_ctx)[i].compression_level;
      }
      
      
    } 
  }
  return 0;
}




CORBA::ULong
TIDorb::core::ziop::Ziop::getLowValue(
                                    const TIDorb::core::PolicyContext& context)
{
  CORBA::Policy_ptr policy = NULL;
  policy = context.getPolicy(ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID);

  if (policy != NULL) {
    ZIOP::CompressionLowValuePolicy_var low_value_policy =
        ZIOP::CompressionLowValuePolicy::_narrow(policy); 

    CORBA::release(policy);

    return low_value_policy->low_value();
  }
  return 0;
}



ZIOP::CompressionMinRatioPolicyValue
TIDorb::core::ziop::Ziop::getMinRatio(
                                    const TIDorb::core::PolicyContext& context)
{
  CORBA::Policy_ptr policy = NULL;
  policy = context.getPolicy(ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID);

  if (policy != NULL) {
    ZIOP::CompressionMinRatioPolicy_var min_ratio_policy =
        ZIOP::CompressionMinRatioPolicy::_narrow(policy); 

    CORBA::release(policy);

    return min_ratio_policy->ratio();
  }
  return 0;
}
