/////////////////////////////////////////////////////////////////////////
//
// File:        QoS.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2005
//
// Revised:
//
// Copyright 2005 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/messaging.h"

#ifndef _TIDORB_CORE_MESSAGING_QOS_H_
#define _TIDORB_CORE_MESSAGING_QOS_H_

namespace TIDorb {
namespace core {
namespace messaging {

class QoS{

public:

  /**
   * Get the RequestEnd time from the RequestEndTimePolicy and 
   * the RelativeRoundtripPolicy
   * @param context
   * @return
   */
  static Messaging::RequestEndTimePolicy_ptr
  getEffectiveRequestEndTimePolicy(const TIDorb::core::PolicyContext&  context);

  /**
   * Checks if the request can be sent and the time while the sender can be
   * waiting for the response.
   * This operation sleeps the time until the RequestStartTimePolicy 
   * allows the invocation.
   * @param orb The ORB     
   * @return the time to wait the response
   * @throws org.omg.CORBA.TIMEOUT if the request time has expired
   */
  static CORBA::ULongLong 
  checkRequestTime(TIDorb::core::TIDORB* orb,
                   const TIDorb::core::PolicyContext& context);
   
  /**
   * @param orb
   * @param currentTime in ns.
   * @return the time to wait the request in ms. (greather or equal than 0)
   */
private:
  static CORBA::ULongLong 
  getEffectiveRequestTimeoutInMillis(const TIDorb::core::PolicyContext& context,
                                     CORBA::ULongLong currentTime);

  /**
   * @param orb
   * @param currentTime
   * @param startTimePolicy
   * @return
   */
  static CORBA::Boolean checkStart(TIDorb::core::TIDORB* orb,
                                   const TIDorb::core::PolicyContext& context,
                                   CORBA::ULongLong currentTime);

  /**
   * Validates QoS RequestEndTimePolicy in the ServerSide
   * @param policySupplier
   * @param trace
   * @return
   */
public:
  static CORBA::Boolean validateServerRequestEndTimePolicy(
                                   const TIDorb::core::PolicyContext& context);

};
}// messaging
}// core
}// TIDorb
#endif
