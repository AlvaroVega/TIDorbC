// //////////////////////////////////////////////////////////////////////////////////
// //
// // File:        ZIOPInvocationPoliciesContext.C
// // Description: 
// //
// // Rel:         01.00
// // Created:     March, 2008
// //
// // Revised:
// //
// // Copyright 2008 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// // The copyright to the software program(s) is property of Telefonica I+D.
// // The program(s) may be used and or copied only with the express written
// // consent of Telefonica I+D or in acordance with the terms and conditions
// // stipulated in the agreement/contract under which the program(s) have
// // been supplied.
// //
// ///////////////////////////////////////////////////////////////////////////////////

// #include "TIDorb/core/ziop/ZIOPInvocationPoliciesContext.h"



// TIDorb::core::ziop::ZIOPInvocationPoliciesContext::ZIOPInvocationPoliciesContext()
//   throw (TIDThr::SystemException)
//   //  : TIDorb::core::comm::iiop::ServiceContext(ZIOP::TAG_ZIOP_INVOCATION_POLICIES)
//   : TIDorb::core::comm::iiop::ServiceContext(IOP::INVOCATION_POLICIES)
// {
//   m_policies = NULL;
// }


// TIDorb::core::ziop::ZIOPInvocationPoliciesContext::ZIOPInvocationPoliciesContext(
//                                                   TIDorb::core::PolicyContext* policies)
//   throw (TIDThr::SystemException)
//   // : TIDorb::core::comm::iiop::ServiceContext(ZIOP::TAG_ZIOP_INVOCATION_POLICIES)
//   : TIDorb::core::comm::iiop::ServiceContext(IOP::INVOCATION_POLICIES)

// {
//   m_policies = policies;
// }


// TIDorb::core::ziop::ZIOPInvocationPoliciesContext::~ZIOPInvocationPoliciesContext()
//   throw (TIDThr::SystemException)
// {
//   if (m_policies != NULL)
//     delete m_policies;
// }

// TIDorb::core::PolicyContext* TIDorb::core::ziop::ZIOPInvocationPoliciesContext::getPolicies()
// {
//   return m_policies;
// }


// void TIDorb::core::ziop::ZIOPInvocationPoliciesContext::partial_read(
//                                                   TIDorb::core::cdr::CDRInputStream& input)
// {
//   cerr << "ZIOPInvocationPoliciesContext::partial_read" << endl;
//   m_policies = new TIDorb::core::PolicyContext(NULL);
  
//   TIDorb::core::cdr::CDRInputStream* encapsulation = input.read_encapsulation();
  
//   try {

//     m_policies->partialRead(*encapsulation);
  
//     delete encapsulation;
    
//   }catch (...) {
//     delete encapsulation;
//     throw;
//   }

// }

	
// void TIDorb::core::ziop::ZIOPInvocationPoliciesContext::write(
//                                                  TIDorb::core::cdr::CDROutputStream& output) const
// {
//   cerr << "ZIOPInvocationPoliciesContext::write" << endl;
//   output.write_ulong(_context_id);
  
//   TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
  
//   try {

//     if(m_policies == NULL) {
//       encapsulation->write_ulong((CORBA::ULong) 0);
//     } else {
//       m_policies->write(*encapsulation);    
//     }
    
//     output.write_buffer(*(encapsulation->get_buffer())); 

//     delete encapsulation;
  	
//   } catch (...) {
//     delete encapsulation;
//     throw;
//   }
// }
	
