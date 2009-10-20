/////////////////////////////////////////////////////////////////////////
//
// File:        PolicyContext.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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

#include "TIDorb/core.h"
#include "TIDorb/core/comm/iiop.h"

namespace TIDorb {
namespace core {


PolicyContext::PolicyContext() 
  throw (TIDThr::SystemException)
  : m_father_context(NULL)
{

}


PolicyContext::~PolicyContext() throw (TIDThr::SystemException) 
{
  m_policy_table.clear();
}

PolicyContext::PolicyContext(TIDorb::core::PolicyContext* father_context)
  : m_father_context(father_context)
{

}

void PolicyContext::setFatherContext(TIDorb::core::PolicyContext* father_context)
{
  m_father_context = father_context;
}

void PolicyContext::setPolicies(const CORBA::PolicyList& policies,
                                CORBA::SetOverrideType set_add)
    throw (CORBA::InvalidPolicies)
{
    TIDThr::Synchronized sync(*this);

    if (set_add == CORBA::SET_OVERRIDE) {
        m_policy_table.clear();
    }

    CORBA::ULong size = policies.length();
    CORBA::InvalidPolicies invalid_ex;

    invalid_ex.indices.length(0);
    CORBA::ULong errors = 0;

    for (CORBA::UShort i = 0; i < size; i++) {
        try {
            setPolicy(policies[i]);
        }
        catch (const CORBA::PolicyError& pe) {
            invalid_ex.indices[errors++] = i;
        }
    }

    if (errors > 0) {
        throw invalid_ex;
    }
}



CORBA::Policy_ptr PolicyContext::getPolicy(CORBA::PolicyType type)  const
{
    TIDThr::Synchronized sync(*((PolicyContext*) this));

    PolicyTableT::const_iterator policy = m_policy_table.find(type);

    if(policy != m_policy_table.end()) {
        return CORBA::Policy::_duplicate((*policy).second);
    } else {
      if (m_father_context != NULL)
        return m_father_context->getPolicy(type);
      else
        return NULL;
    }
}




CORBA::PolicyList* PolicyContext::getPolicies(const CORBA::PolicyTypeSeq& ts) const
{
    TIDThr::Synchronized sync(*((PolicyContext*) this));

    CORBA::PolicyList* polist = new CORBA::PolicyList();

    CORBA::Policy_ptr policy = NULL;
    CORBA::ULong size = ts.length();
    CORBA::ULong index = 0;

    if (size == 0) {
      PolicyTableT::const_iterator it  = m_policy_table.begin();
      PolicyTableT::const_iterator end = m_policy_table.end();
      while (it != end) {
        polist->length(index + 1);
        (*polist)[index++] = CORBA::Policy::_duplicate(it->second);
        it++;
      }
    } else {
      for (CORBA::ULong i = 0; i < size; i++) {
        policy = getPolicy(ts[i]);
        if (policy) {
          polist->length(index + 1);
          (*polist)[index++] = policy;
          CORBA::release(policy);
        }
      }
    }
    return polist;
}




void PolicyContext::setPolicy(CORBA::Policy_ptr policy)
{
    TIDThr::Synchronized sync(*this);

    if (!policy) {
        throw CORBA::BAD_PARAM("Null Policy reference");
    }

    CORBA::PolicyType type = policy->policy_type();
    PolicyFactory::validate_for_orb(type);

    m_policy_table[type] = CORBA::Policy::_duplicate(policy);
}




void PolicyContext::removePolicy(CORBA::PolicyType type)
{
    TIDThr::Synchronized sync(*this);

    m_policy_table.erase(type);
}




void PolicyContext::reset()
{
    TIDThr::Synchronized sync(*this);
    m_policy_table.clear();
}




PolicyContext* PolicyContext::duplicate()
{
  TIDThr::Synchronized sync(*this);
 
  PolicyContext* copy = new PolicyContext(m_father_context);
  
  PolicyTableT::iterator it  = m_policy_table.begin();
  PolicyTableT::iterator end = m_policy_table.end();
  while (it != end) {
    copy->setPolicy(it->second);
    it++;
  }
  
  return copy;
}


/**
 * Reads the policies as a pair PolicyType/encapsuled policy data
 * @param input
 */
void PolicyContext::partialRead(TIDorb::core::cdr::CDRInputStream& input)
{        
  CORBA::ULong size;
  input.read_ulong(size);
  
  CORBA::Policy_ptr policy = NULL;

  for (CORBA::ULong i = 0; i < size; i++) {
    policy = TIDorb::core::PolicyFactory::readPolicy(input);
    if(policy != NULL) {   
      try {
        setPolicy(policy);
        CORBA::release(policy);
      }
      catch (const CORBA::PolicyError& e) {
        //unreachable
      }
      catch (...) {
      }
    }
  }        
}



/**
 * Writes the policies as a pair PolicyType/encalsuled policy data
 * @param output the outputstream
 */
void PolicyContext::write(TIDorb::core::cdr::CDROutputStream& output) const
{        
  output.write_ulong(m_policy_table.size());

  PolicyTableT::const_iterator it = m_policy_table.begin();
  PolicyTableT::const_iterator end = m_policy_table.end();


  while(it != end){

    switch((*it).first){
    case Messaging::REBIND_POLICY_TYPE: 
      {
        Messaging::RebindPolicy_ptr policy = 
          Messaging::RebindPolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        encapsulation->write_short(policy->rebind_mode());
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete encapsulation;
        break;
      }
    case Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE: 
      {
        Messaging::RelativeRequestTimeoutPolicy_ptr policy = 
          Messaging::RelativeRequestTimeoutPolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        encapsulation->write_ulonglong(policy->relative_expiry());
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete encapsulation;
        break;
      }
    case Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE:
      {
        Messaging::RelativeRoundtripTimeoutPolicy_ptr policy = 
          Messaging::RelativeRoundtripTimeoutPolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        encapsulation->write_ulonglong(policy->relative_expiry());
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete encapsulation;
        break;
      }
    case Messaging::REQUEST_START_TIME_POLICY_TYPE:
      {
        Messaging::RequestStartTimePolicy_ptr policy = 
          Messaging::RequestStartTimePolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        encapsulation->write_ulonglong(policy->start_time().time);
        encapsulation->write_ulong(policy->start_time().inacclo);
        encapsulation->write_ushort(policy->start_time().inacchi);
        encapsulation->write_short(policy->start_time().tdf);
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete encapsulation;
        break;
      }
    case Messaging::REQUEST_PRIORITY_POLICY_TYPE:
      {
        Messaging::RequestPriorityPolicy_ptr policy = 
          Messaging::RequestPriorityPolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        encapsulation->write_short(policy->priority_range().min);
        encapsulation->write_short(policy->priority_range().max);
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete encapsulation;
        break;
      }
    case Messaging::REQUEST_END_TIME_POLICY_TYPE:
      {
        Messaging::RequestEndTimePolicy_ptr policy = 
          Messaging::RequestEndTimePolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        encapsulation->write_ulonglong(policy->end_time().time);
        encapsulation->write_ulong(policy->end_time().inacclo);
        encapsulation->write_ushort(policy->end_time().inacchi);
        encapsulation->write_short(policy->end_time().tdf);
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete encapsulation;
        break;
      }
    case Messaging::QUEUE_ORDER_POLICY_TYPE:
      {
        Messaging::QueueOrderPolicy_ptr policy = 
          Messaging::QueueOrderPolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        encapsulation->write_ushort(policy->allowed_orders());
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete encapsulation;
        break;
      }
    case BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE:
      {
        BiDirPolicy::BidirectionalPolicy_ptr policy = 
          BiDirPolicy::BidirectionalPolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        encapsulation->write_ushort(policy->value());
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete encapsulation;
        break;
      }
    case ZIOP::COMPRESSION_ENABLING_POLICY_ID: 
      {
        ZIOP::CompressionEnablingPolicy_ptr policy = 
          ZIOP::CompressionEnablingPolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        encapsulation->write_boolean(policy->compression_enabled());
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete encapsulation;
        break;
      }
    case ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID: 
      {
        ZIOP::CompressorIdLevelListPolicy_ptr policy = 
          ZIOP::CompressorIdLevelListPolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        Compression::CompressorIdLevelList* compressors = policy->compressor_ids();
        CORBA::ULong length = compressors->length();
        encapsulation->write_ulong(length);
        for (CORBA::ULong i = 0; i < length; i++){
          encapsulation->write_ushort((*compressors)[i].compressor_id);
          encapsulation->write_ushort((*compressors)[i].compression_level);
        }
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete compressors;
        delete encapsulation;
        break;
      }
    case ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID:
      {
        ZIOP::CompressionLowValuePolicy_ptr policy = 
          ZIOP::CompressionLowValuePolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        encapsulation->write_ulong(policy->low_value());
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete encapsulation;    
        break;
      }
    case ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID:
      {
        ZIOP::CompressionMinRatioPolicy_ptr policy = 
          ZIOP::CompressionMinRatioPolicy::_narrow((*it).second);
        output.write_ulong(policy->policy_type());
        TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
        encapsulation->write_long(policy->ratio());
        output.write_buffer(*(encapsulation->get_buffer()));
        CORBA::release(policy);
        delete encapsulation;
        break;
      }

    default:
      {
        // Never reached or policy not supported: nothing to do anywhere
      }

    } // case
    it++;
  } // while



}





void PolicyContext::dump(ostream& out)
{
  TIDThr::Synchronized sync(*this);

  PolicyTableT::const_iterator it = m_policy_table.begin();
  PolicyTableT::const_iterator end = m_policy_table.end();


  while(it != end){
    
    switch((*it).first){
    case Messaging::REBIND_POLICY_TYPE: 
      {
        Messaging::RebindPolicy_ptr policy = 
          Messaging::RebindPolicy::_narrow((*it).second);
        out << "\t\tMESSAGING::REBIND_POLICY" << "=";
        out << policy->rebind_mode() << endl;
        CORBA::release(policy);
        break;
      }
    case Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE: 
      {
        Messaging::RelativeRequestTimeoutPolicy_ptr policy = 
          Messaging::RelativeRequestTimeoutPolicy::_narrow((*it).second);
        out << "\t\tMESSAGING::RELATIVE_REQ_TIMEOUT_POLICY" << "=";
        out << policy->relative_expiry() << " ms" << endl;
        CORBA::release(policy);
        break;
      }
    case Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE:
      {
        Messaging::RelativeRoundtripTimeoutPolicy_ptr policy = 
          Messaging::RelativeRoundtripTimeoutPolicy::_narrow((*it).second);
        out << "\t\tMESSAGING::RELATIVE_RT_TIMEOUT_POLICY" << "="; 
        out << TIDorb::core::util::Time::toTimeInMillis(policy->relative_expiry());
        out << " ms " << endl;
        CORBA::release(policy);
        break;
      }
    case Messaging::REQUEST_START_TIME_POLICY_TYPE:
      {
        Messaging::RequestStartTimePolicy_ptr policy = 
          Messaging::RequestStartTimePolicy::_narrow((*it).second);
        out << "\t\tMESSAGING::REQUEST_START_TIME" << "=";
//         encapsulation->write_ulonglong(policy->start_time().time);
//         encapsulation->write_ulong(policy->start_time().inacclo);
//         encapsulation->write_ushort(policy->start_time().inacchi);
//         encapsulation->write_short(policy->start_time().tdf);
        out << endl;
        CORBA::release(policy);
        break;
      }
    case Messaging::REQUEST_PRIORITY_POLICY_TYPE:
      {
        Messaging::RequestPriorityPolicy_ptr policy = 
          Messaging::RequestPriorityPolicy::_narrow((*it).second);
        out << "\t\tMESSAGING::REQUEST_PRIORITY_POLICY_TYPE" << "=";
//         encapsulation->write_short(policy->priority_range().min);
//         encapsulation->write_short(policy->priority_range().max);
        out << endl;
        CORBA::release(policy);
        break;
      }
    case Messaging::REQUEST_END_TIME_POLICY_TYPE:
      {
        Messaging::RequestEndTimePolicy_ptr policy = 
          Messaging::RequestEndTimePolicy::_narrow((*it).second);
        out << "\t\tMESSAGING::REQUEST_END_TIME_POLICY_TYPE" << "=";
//         encapsulation->write_ulonglong(policy->end_time().time);
//         encapsulation->write_ulong(policy->end_time().inacclo);
//         encapsulation->write_ushort(policy->end_time().inacchi);
//         encapsulation->write_short(policy->end_time().tdf);
        out << endl;
        CORBA::release(policy);
        break;
      }
    case Messaging::QUEUE_ORDER_POLICY_TYPE:
      {
        Messaging::QueueOrderPolicy_ptr policy = 
          Messaging::QueueOrderPolicy::_narrow((*it).second);
        out << "\t\tMESSAGING::QUEUE_ORDER_POLICY" << "=";
        out << policy->allowed_orders() << endl;
        CORBA::release(policy);
        break;
      }
    case BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE:
      {
        BiDirPolicy::BidirectionalPolicy_ptr policy = 
          BiDirPolicy::BidirectionalPolicy::_narrow((*it).second);
        out << "\t\tBiDirPolicy::BIDIRECTIONAL_POLICY" << "=";
        if (policy->value() == BiDirPolicy::NORMAL)
          out << "NORMAL" << endl;
        else
          out << "BOTH" << endl;
        CORBA::release(policy);
        break;
      }
    default:
      {
        // Never reached or policy not supported: nothing to do anywhere
      }

    } // case
    it++;
  } // while

}



size_t PolicyContext::getSize(){
  return m_policy_table.size();
}


TIDorb::core::comm::iiop::InvocationPoliciesContext* 
PolicyContext::getInvocationPolicyServiceContext()
{
  Messaging::RequestEndTimePolicy_ptr endPolicy = 
    TIDorb::core::messaging::QoS::getEffectiveRequestEndTimePolicy(
                                               (const TIDorb::core::PolicyContext&)*this);


  CORBA::Policy_ptr policy = 
    this->getPolicy(Messaging::REQUEST_PRIORITY_POLICY_TYPE);

  if( (endPolicy != NULL) || ( policy != NULL) ){
    PolicyContext* invocationPolicies = new PolicyContext(NULL);
    if(endPolicy != NULL) {
      invocationPolicies->setPolicy(endPolicy);
      CORBA::release(endPolicy);
    }
    if(policy != NULL) {
      Messaging::RequestPriorityPolicy_ptr priorityPolicy = 
        Messaging::RequestPriorityPolicy::_narrow(policy);
      invocationPolicies->setPolicy(priorityPolicy);
      CORBA::release(policy);
      CORBA::release(priorityPolicy);
    }
    return new TIDorb::core::comm::iiop::InvocationPoliciesContext(invocationPolicies);
  } else {
    return NULL;
  }        

}





} //core
} //TIDorb
