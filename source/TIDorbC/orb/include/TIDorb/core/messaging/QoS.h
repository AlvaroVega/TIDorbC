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
