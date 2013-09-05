/////////////////////////////////////////////////////////////////////////
//
// File:        SASManager.h
// Description:
//              
// Rel:         01.00
// Created:     September, 2009
//
// Revised:
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/security/sas.h"

#ifndef _TIDORB_CORE_SECURITY_SAS_SAS_MANAGER_H_
#define _TIDORB_CORE_SECURITY_SAS_SAS_MANAGER_H_


#include <map>

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#undef __STD_ISTREAM__
#undef __STD_OSTREAM__
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
#endif

#include "CSI.h"
#include "GSSUP.h"

namespace TIDorb {
namespace core {
namespace security {
namespace sas {

/*
 * SASManager at CSIv2 level 0
 */


class SASManager : public TIDThr::RecursiveMutex
{

protected:    	

  TIDorb::core::TIDORB* m_orb;

  IOP::Codec_var m_codec;

  const TIDorb::core::ConfORB& conf;

private:


  // ContextId from Session of TransportLayer (IIOP -> 0 (stateless?), 
  //                                           SSLIIOP -> getSesionid() || 0)
  // �SASContextBody or SASContext?
  typedef map<CSI::ContextId, CSI::EstablishContext> EstablishedContextTable; 

  EstablishedContextTable _established_contexts;


public:

  SASManager(TIDorb::core::TIDORB* orb)
    throw (TIDThr::SystemException);


  ~SASManager()
    throw (TIDThr::SystemException);




  //
  // CSS side
  //

  TIDorb::core::security::sas::SASServiceContext*
  get_client_SASServiceContext(TIDorb::core::iop::IOR* ior);


  //
  // TSS side
  //
  CSI::SASContextBody_ptr validate_SASContextBody(CSI::SASContextBody_ptr body);



  //
  // Generic GSS Token
  //
  CSI::GSSToken_ptr 
  create_GSSUP_GSSToken(const char* username, const char* password, 
                        const char* target_name);
  
  CORBA::Boolean
  validate_GSSToken(const CSI::GSSToken& gss_token,
                    CSI::GSSToken_ptr& gss_token_response);


  //
  // GSSUP token 
  // 
  static GSSUP::InitialContextToken_ptr 
  create_GSSUP_ic_token(const char* username, const char* password, 
                        const char* target_name);

 


  GSSUP::ErrorCode
  validate_GSSUP_token(const CORBA::OctetSeq_ptr ic_token_buffer, 
                       const char* username, const char* password,
                       const char* target_name);
  
  const char* get_GSSToken_error_reason(const CSI::GSSToken& gss_token);


  //private:
  CSI::ContextId check_equivalent_context(TIDorb::core::iop::IOR* ior);


  //
  // Common utilities
  //
  static CORBA::OctetSeq_ptr convert_oid_to_asn1(const char* oid);

  static char* convert_asn1_to_oid(CORBA::OctetSeq& oid_asn1);

  static CORBA::OctetSeq_ptr convert_length_to_octets(CORBA::ULong length);

  static CORBA::ULong convert_octets_to_length(const CORBA::OctetSeq& buffer,
                                               CORBA::ULong index);

  static CORBA::ULong get_num_octets_from_octets(const CORBA::OctetSeq& buffer,
                                                 CORBA::ULong index);

  static CORBA::OctetSeq_ptr convert_int_to_asn1char(CORBA::ULong length);

};
 
} // sas
} // security
} // core
} // TIDorb
#endif

