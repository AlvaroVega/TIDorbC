/////////////////////////////////////////////////////////////////////////
//
// File:        QoS.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2005
//
// Revised:
//
// Copyright 2005 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/messaging/PoliciesComponent.h"

/**
 * Get the RequestEnd time from the RequestEndTimePolicy and 
 * the RelativeRoundtripPolicy
 * @param context
 * @return
 */
Messaging::RequestEndTimePolicy_ptr
TIDorb::core::messaging::QoS::getEffectiveRequestEndTimePolicy(
                                            const TIDorb::core::PolicyContext& context){

  CORBA::Policy_ptr policy1 = NULL;
  policy1 = context.getPolicy(Messaging::REQUEST_END_TIME_POLICY_TYPE);

  CORBA::Policy_ptr policy2 = NULL;
  policy2 = context.getPolicy(Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE);

  
  if (policy2 == NULL) {
 
    if (policy1 != NULL) {
      Messaging::RequestEndTimePolicy_ptr policy_ret = 
        Messaging::RequestEndTimePolicy::_narrow(policy1); 
      CORBA::release(policy1);
      return policy_ret;
    }
    else
      return NULL;

  } else{

    Messaging::RequestEndTimePolicy_ptr requestEndTimePolicy = NULL;
    Messaging::RelativeRoundtripTimeoutPolicy_var relativeRoundtripTimeoutPolicy = NULL;

    if (policy1 != NULL) {
      requestEndTimePolicy = 
        Messaging::RequestEndTimePolicy::_narrow(policy1);
      CORBA::release(policy1);
    }
    
    relativeRoundtripTimeoutPolicy =
      Messaging::RelativeRoundtripTimeoutPolicy::_narrow(policy2); 
    CORBA::release(policy2);

    CORBA::ULongLong endTime = 
      TIDorb::core::util::Time::toTimeT(TIDorb::core::util::Time::currentUtcTimeMillis());
    endTime += relativeRoundtripTimeoutPolicy->relative_expiry();
    
    if((requestEndTimePolicy != NULL) && 
       (endTime > requestEndTimePolicy->end_time().time) ) {
      return requestEndTimePolicy;                
    } else 
      return new RequestEndTimePolicyImpl(endTime);
  }        

}

/**
 * Checks if the request can be sent and the time while the sender can be
 * waiting for the response.
 * This operation sleeps the time until the RequestStartTimePolicy 
 * allows the invocation.
 * @param orb The ORB     
 * @return the time to wait the response
 * @throws org.omg.CORBA.TIMEOUT if the request time has expired
 */
CORBA::ULongLong 
TIDorb::core::messaging::QoS::checkRequestTime(TIDorb::core::TIDORB* orb,
                                               const TIDorb::core::PolicyContext& context){
  
  CORBA::Policy_ptr policy = NULL;
  policy = context.getPolicy(Messaging::REQUEST_START_TIME_POLICY_TYPE);

  CORBA::ULongLong currentTime = 0;    
  
  if (policy != NULL) {
    currentTime = TIDorb::core::util::Time::currentUtcTimeMillis();
    checkStart(orb, context, currentTime); 
    CORBA::release(policy);
  }
  
  CORBA::ULongLong requestTimeout = 
    getEffectiveRequestTimeoutInMillis(context, currentTime);     
  
  
  if (requestTimeout < 0) {

    if (orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "QoS RequestEndTimePolicy/RelativeRountripTimeoutPolicy ";
      msg << "exceeded in " << requestTimeout << " ms" << endl; 
      orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
    throw CORBA::TIMEOUT();
  } else {
    return requestTimeout;
  }
}

/**
 * @param orb
 * @param currentTime in ns.
 * @return the time to wait the request in ms. (greather or equal than 0)
 */
CORBA::ULongLong 
TIDorb::core::messaging::QoS::getEffectiveRequestTimeoutInMillis(
                                             const TIDorb::core::PolicyContext& context,
                                             CORBA::ULongLong currentTime){

  CORBA::Policy_ptr policy1 = NULL;
  policy1 = context.getPolicy(Messaging::REQUEST_END_TIME_POLICY_TYPE);

  CORBA::Policy_ptr policy2 = NULL;
  policy2 = context.getPolicy(Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE);

  Messaging::RequestEndTimePolicy_var requestEndTimePolicy = NULL;
  Messaging::RelativeRoundtripTimeoutPolicy_var relativeRoundtripTimeoutPolicy = NULL;
  
  if (policy2 != NULL) {
    relativeRoundtripTimeoutPolicy =
      Messaging::RelativeRoundtripTimeoutPolicy::_narrow(policy2); 
  }

         
  if (policy1 == NULL){

    if (policy2 != NULL) {
      CORBA::release(policy2);
      return TIDorb::core::util::Time::toTimeInMillis(
                                 relativeRoundtripTimeoutPolicy->relative_expiry());
    } else 
      return 0;

  } else {

    if (currentTime == 0) 
      currentTime = TIDorb::core::util::Time::currentUtcTimeMillis();
    
    requestEndTimePolicy = 
      Messaging::RequestEndTimePolicy::_narrow(policy1);
    CORBA::release(policy1);

    CORBA::ULongLong endTimeout = 
      TIDorb::core::util::Time::toTimeInMillis(requestEndTimePolicy->end_time().time) - 
      currentTime; 
    
    if (policy2 != NULL) {
      CORBA::ULongLong relativeTimeout = 
        TIDorb::core::util::Time::toTimeInMillis(
                             relativeRoundtripTimeoutPolicy->relative_expiry());
      CORBA::release(policy2);
      return (endTimeout < relativeTimeout)? endTimeout : relativeTimeout;
    } else {
      return endTimeout;
    }
  }        
  

}

/**
 * @param orb
 * @param currentTime
 * @param startTimePolicy
 * @return
 */
CORBA::Boolean TIDorb::core::messaging::QoS::checkStart(
                                            TIDorb::core::TIDORB* orb,
                                            const TIDorb::core::PolicyContext& context,
                                            CORBA::ULongLong currentTime){

  
  CORBA::Policy_ptr policy = NULL;
  policy = context.getPolicy(Messaging::REQUEST_START_TIME_POLICY_TYPE);

  Messaging::RequestStartTimePolicy_ptr requestStartTimePolicy = NULL;

  requestStartTimePolicy =
   Messaging::RequestStartTimePolicy::_narrow(policy); 
  CORBA::release(policy);

  // UtcT.time is in order of 100 ns. (1 ns = 10^6 ms.) 100 ns = 10^4
        
  CORBA::ULongLong startTime = 
    TIDorb::core::util::Time::toTimeInMillis(requestStartTimePolicy->start_time().time);
  CORBA::release(requestStartTimePolicy);

  CORBA::ULongLong sleepMillis = startTime - currentTime;
        
  if (sleepMillis > 0) {           
            
    try {
      TIDThr::Thread::sleep(sleepMillis);
    }
    catch (const TIDThr::InterruptedException& e) {

      if (orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "QoS: Request has received an InterruptedException ";
        
        orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
      }

    }  

    if (orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "QoS: Request has slept " << sleepMillis;
      msg << " ms. util Messaging::RequestStartTimePolicy value";
      orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
    
    return true;
  }
  
  return false;
  
}

/**
 * Validates QoS RequestEndTimePolicy in the ServerSide
 * @param policySupplier
 * @param trace
 * @return
 */
CORBA::Boolean 
TIDorb::core::messaging::QoS::validateServerRequestEndTimePolicy(
                                            const TIDorb::core::PolicyContext& context){

  CORBA::Policy_ptr policy = NULL;
  policy = context.getPolicy(Messaging::REQUEST_END_TIME_POLICY_TYPE);

  if(policy == NULL)
    return true;            

  else{            
    
    Messaging::RequestEndTimePolicy_ptr requestEndTimePolicy = NULL;
    
    requestEndTimePolicy =
      Messaging::RequestEndTimePolicy::_narrow(policy); 
    CORBA::release(policy);

    CORBA::ULongLong current_time = TIDorb::core::util::Time::currentUtcTimeMillis(); 
    TimeBase::UtcT utc_time;
    utc_time = requestEndTimePolicy->end_time();

    CORBA::release(requestEndTimePolicy);

    // Request TIMEOUT?
    if(current_time > TIDorb::core::util::Time::toTimeInMillis(utc_time.time))  {
                
      return false;
    }
            
    return true;
  }        


}

