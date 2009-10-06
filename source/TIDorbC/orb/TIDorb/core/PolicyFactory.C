/////////////////////////////////////////////////////////////////////////
//
// File:        PolicyFactory.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"
#include "TIDorb/core/poa/policies.h"




CORBA::Policy_ptr 
TIDorb::core::PolicyFactory::create_policy(CORBA::PolicyType type, 
                                           const CORBA::Any& val)
  throw (CORBA::PolicyError)
{
  switch(type) {
    case PortableServer::THREAD_POLICY_ID:
	return create_ThreadPolicy(val);
    case PortableServer::LIFESPAN_POLICY_ID:
	return create_LifespanPolicy(val);
    case PortableServer::ID_UNIQUENESS_POLICY_ID:
	return create_IdUniquenessPolicy(val);
    case PortableServer::ID_ASSIGNMENT_POLICY_ID:
	return create_IdAssignmentPolicy(val);
    case PortableServer::SERVANT_RETENTION_POLICY_ID:
	return create_ServantRetentionPolicy(val);
    case PortableServer::REQUEST_PROCESSING_POLICY_ID:
	return create_RequestProcessingPolicy(val);
    case BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE:
	return create_BidirectionalPolicy(val);    
    // FT extensions
    case FT::HEARTBEAT_POLICY:
	return create_HeartbeatPolicy(val);    
    case FT::HEARTBEAT_ENABLED_POLICY:
	return create_HeartbeatEnabledPolicy(val);    
    // QoS extensions
    case Messaging::REBIND_POLICY_TYPE:
      return create_RebindPolicyImpl(val);
    case Messaging::REQUEST_PRIORITY_POLICY_TYPE:
      return create_RequestPriorityPolicyImpl(val);
    case Messaging::REQUEST_START_TIME_POLICY_TYPE:
      return create_RequestStartTimePolicyImpl(val);
    case Messaging::REQUEST_END_TIME_POLICY_TYPE:
      return create_RequestEndTimePolicyImpl(val);
    case Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE:
      return create_RelativeRequestTimeoutPolicyImpl(val);
    case Messaging::QUEUE_ORDER_POLICY_TYPE:
      return create_QueueOrderPolicyImpl(val);
    case Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE:
      return create_RelativeRoundtripTimeoutPolicyImpl(val);
    // ZIOP extensions
    case ZIOP::COMPRESSION_ENABLING_POLICY_ID: 
      return create_CompressionEnablingPolicyImpl(val);
    case ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID: 
       return create_CompressorIdLevelListPolicyImpl(val);
    case ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID:
       return create_CompressionLowValuePolicyImpl(val);
    case ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID:
       return create_CompressionMinRatioPolicyImpl(val);
    default:
	throw CORBA::PolicyError(CORBA::UNSUPPORTED_POLICY);
  }
}



void TIDorb::core::PolicyFactory::validate_for_orb(const CORBA::PolicyType type)
  throw (CORBA::PolicyError)
{
  switch(type) {
    case PortableServer::THREAD_POLICY_ID:
    case PortableServer::LIFESPAN_POLICY_ID:
    case PortableServer::ID_UNIQUENESS_POLICY_ID:
    case PortableServer::ID_ASSIGNMENT_POLICY_ID:
    case PortableServer::SERVANT_RETENTION_POLICY_ID:
    case PortableServer::REQUEST_PROCESSING_POLICY_ID:
    case BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE:
    // FT extensions
    case FT::HEARTBEAT_POLICY:
    case FT::HEARTBEAT_ENABLED_POLICY:
    // QoS extensions
    case Messaging::REBIND_POLICY_TYPE:
    case Messaging::REQUEST_PRIORITY_POLICY_TYPE:
    case Messaging::REQUEST_START_TIME_POLICY_TYPE:
    case Messaging::REQUEST_END_TIME_POLICY_TYPE:
    case Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE:
    case Messaging::QUEUE_ORDER_POLICY_TYPE:
    case Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE:
    // TODO: ZIOP extensions
    case ZIOP::COMPRESSION_ENABLING_POLICY_ID: 
    case ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID: 
    case ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID:
    case ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID:
        return;
    default:
        throw CORBA::PolicyError(CORBA::UNSUPPORTED_POLICY);
  }
}




TIDorb::core::poa::policies::ThreadPolicyImpl* 
TIDorb::core::PolicyFactory::create_ThreadPolicy(const CORBA::Any& val)
  throw (CORBA::PolicyError) 
{
  PortableServer::ThreadPolicyValue value;
	
  if(val >>= value)
    return new TIDorb::core::poa::policies::ThreadPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}




TIDorb::core::poa::policies::LifespanPolicyImpl* 
TIDorb::core::PolicyFactory::create_LifespanPolicy(const CORBA::Any& val)
  throw (CORBA::PolicyError) 
{
  PortableServer::LifespanPolicyValue value;
  
  if(val >>= value)
    return new TIDorb::core::poa::policies::LifespanPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}




TIDorb::core::poa::policies::IdUniquenessPolicyImpl* 
TIDorb::core::PolicyFactory::create_IdUniquenessPolicy(const CORBA::Any& val)
  throw (CORBA::PolicyError) 
{
  PortableServer::IdUniquenessPolicyValue value;

  if(val >>= value)
    return new TIDorb::core::poa::policies::IdUniquenessPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}




TIDorb::core::poa::policies::IdAssignmentPolicyImpl* 
TIDorb::core::PolicyFactory::create_IdAssignmentPolicy(const CORBA::Any& val)
  throw (CORBA::PolicyError) 	
{
  PortableServer::IdAssignmentPolicyValue value;

  if(val >>= value)
    return new TIDorb::core::poa::policies::IdAssignmentPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}




TIDorb::core::poa::policies::ServantRetentionPolicyImpl* 
TIDorb::core::PolicyFactory::create_ServantRetentionPolicy(const CORBA::Any& val)
  throw (CORBA::PolicyError) 
{
  PortableServer::ServantRetentionPolicyValue value;

  if(val >>= value)
    return new TIDorb::core::poa::policies::ServantRetentionPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}




TIDorb::core::poa::policies::RequestProcessingPolicyImpl* 
TIDorb::core::PolicyFactory::create_RequestProcessingPolicy(const CORBA::Any& val)
  throw (CORBA::PolicyError) 
{
  PortableServer::RequestProcessingPolicyValue value;
	
  if(val >>= value)
    return new TIDorb::core::poa::policies::RequestProcessingPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}




TIDorb::core::BidirectionalPolicyImpl* 
TIDorb::core::PolicyFactory::create_BidirectionalPolicy(const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  BiDirPolicy::BidirectionalPolicyValue value;
  
  if(val >>= value) {
    if((value != BiDirPolicy::BOTH) && (value != BiDirPolicy::NORMAL))
      throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
      
    return new BidirectionalPolicyImpl(value);
  } else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}



//
// FT extensions
//

TIDorb::core::HeartbeatPolicyImpl* 
TIDorb::core::PolicyFactory::create_HeartbeatPolicy(const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  const ::FT::HeartbeatPolicyValue* value = NULL;
	
  if(val >>= value)
    return new HeartbeatPolicyImpl(*value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}




TIDorb::core::HeartbeatEnabledPolicyImpl* 
TIDorb::core::PolicyFactory::create_HeartbeatEnabledPolicy(const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  CORBA::Boolean value;
	
  if(val >>= value)
    return new HeartbeatEnabledPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}



//
// QoS extensions
//

TIDorb::core::messaging::QueueOrderPolicyImpl* 
TIDorb::core::PolicyFactory::create_QueueOrderPolicyImpl(const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  Messaging::Ordering value;
	
  if(val >>= value)
    return new TIDorb::core::messaging::QueueOrderPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}


TIDorb::core::messaging::RebindPolicyImpl* 
TIDorb::core::PolicyFactory::create_RebindPolicyImpl(const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  Messaging::RebindMode value;
	
  if(val >>= value)
    return new TIDorb::core::messaging::RebindPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}


TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl* 
TIDorb::core::PolicyFactory::create_RelativeRequestTimeoutPolicyImpl(
                                                                   const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  TimeBase::TimeT value;
	
  if(val >>= value)
    return new TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}


TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl* 
TIDorb::core::PolicyFactory::create_RelativeRoundtripTimeoutPolicyImpl(
                                                                    const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
   TimeBase::TimeT value;
	
  if(val >>= value)
    return new TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}


TIDorb::core::messaging::RequestEndTimePolicyImpl* 
TIDorb::core::PolicyFactory::create_RequestEndTimePolicyImpl(const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  const TimeBase::UtcT* value = NULL;
	
  if(val >>= value)
    return new TIDorb::core::messaging::RequestEndTimePolicyImpl(*value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}


TIDorb::core::messaging::RequestPriorityPolicyImpl* 
TIDorb::core::PolicyFactory::create_RequestPriorityPolicyImpl(
                                                             const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  const Messaging::PriorityRange* value = NULL;
	
  if(val >>= value)
    return new TIDorb::core::messaging::RequestPriorityPolicyImpl(*value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}


TIDorb::core::messaging::RequestStartTimePolicyImpl* 
TIDorb::core::PolicyFactory::create_RequestStartTimePolicyImpl(
                                                             const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  const TimeBase::UtcT* value = NULL;
	
  if(val >>= value)
    return new TIDorb::core::messaging::RequestStartTimePolicyImpl(*value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}


TIDorb::core::ziop::CompressionEnablingPolicyImpl*
TIDorb::core::PolicyFactory::create_CompressionEnablingPolicyImpl(
                                                             const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  ZIOP::CompressionEnablingPolicyValue value;
	
  if(val >>= value)
    return new TIDorb::core::ziop::CompressionEnablingPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}


TIDorb::core::ziop::CompressorIdLevelListPolicyImpl*
TIDorb::core::PolicyFactory::create_CompressorIdLevelListPolicyImpl(
                                                             const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  Compression::CompressorIdLevelList* value;
	
  if(val >>= value)
    return new TIDorb::core::ziop::CompressorIdLevelListPolicyImpl(*value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}


TIDorb::core::ziop::CompressionLowValuePolicyImpl*
TIDorb::core::PolicyFactory::create_CompressionLowValuePolicyImpl(
                                                             const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  ZIOP::CompressionLowValuePolicyValue value;

  if(val >>= value)
    return new TIDorb::core::ziop::CompressionLowValuePolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}

TIDorb::core::ziop::CompressionMinRatioPolicyImpl*
TIDorb::core::PolicyFactory::create_CompressionMinRatioPolicyImpl(
                                                             const CORBA::Any& val)
  throw(CORBA::PolicyError) 
{
  ZIOP::CompressionMinRatioPolicyValue value;

  if(val >>= value)
    return new TIDorb::core::ziop::CompressionMinRatioPolicyImpl(value);
  else
    throw CORBA::PolicyError(CORBA::BAD_POLICY_VALUE);
}



/**
 * 
 * Reads a policy from an INVOCATION_POLICY ServiceContext encapsulation.
 * See QoS section: "22.3 Propagation of Messaging QoS"
 * 
 * @param input
 * @return
 */
CORBA::Policy_ptr 
TIDorb::core::PolicyFactory::readPolicy(TIDorb::core::cdr::CDRInputStream& input){

  CORBA::PolicyType policy_type;
  input.read_ulong(policy_type);
  CORBA::Policy_ptr policy = NULL;

  TIDorb::core::cdr::CDRInputStream* encapsulation = NULL; 
      
  switch (policy_type) {
  case BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE:
    {
      encapsulation = input.read_encapsulation();
      policy = TIDorb::core::BidirectionalPolicyImpl::read(*encapsulation);   
      break; 
    }           
  case Messaging::REQUEST_END_TIME_POLICY_TYPE: 
    {
      encapsulation = input.read_encapsulation();
      policy = TIDorb::core::messaging::RequestEndTimePolicyImpl::read(*encapsulation);
      break;
    }
  case Messaging::REQUEST_START_TIME_POLICY_TYPE: 
    {
      encapsulation = input.read_encapsulation();
      policy = TIDorb::core::messaging::RequestStartTimePolicyImpl::read(*encapsulation);
      break;
    }
  case Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE: 
    {
      encapsulation = input.read_encapsulation();
      policy = TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::read(*encapsulation);
      break;
    }
  case Messaging::REQUEST_PRIORITY_POLICY_TYPE: 
    {
      encapsulation = input.read_encapsulation();
      policy = TIDorb::core::messaging::RequestPriorityPolicyImpl::read(*encapsulation);
      break; 
    }
  case ZIOP::COMPRESSION_ENABLING_POLICY_ID: 
    {
      encapsulation = input.read_encapsulation();
      policy = TIDorb::core::ziop::CompressionEnablingPolicyImpl::read(*encapsulation);
      break; 
    }
  case ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID: 
    {
      encapsulation = input.read_encapsulation();
      policy = TIDorb::core::ziop::CompressorIdLevelListPolicyImpl::read(*encapsulation);
      break; 
    }
  default: // policy not supported skip               
    {
      input.skip_encapsulation();
      return NULL;        
    }
  }

  delete encapsulation;
        
  return policy;

}
