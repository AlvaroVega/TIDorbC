// /////////////////////////////////////////////////////////////////////////
// //
// // File:        ZIOPPoliciesComponent.h
// // Description:
// //              
// // Rel:         01.00
// // Created:     March, 2008
// //
// // Revised:
// //
// // Copyright 2008 Telefonica, I+D. Printed in Spain (Europe). All Right
// // Reserved.
// // The copyright to the software program(s) is property of Telefonica I+D.
// // The program(s) may be used and or copied only with the express written
// // consent of Telefonica I+D or in acordance with the terms and conditions
// // stipulated in the agreement/contract under which the program(s) have
// // been supplied.
// //
// /////////////////////////////////////////////////////////////////////////

// #include "TIDorb/core/ziop.h"

// #ifndef _TIDORB_CORE_ZIOP_POLICIES_COMPONENT_H_
// #define _TIDORB_CORE_ZIOP_POLICIES_COMPONENT_H_

// #include "CORBA.h"
// #include "ZIOP.h"


// namespace TIDorb {
// namespace core {
// namespace ziop {

// class ZIOPPoliciesComponent: public TIDorb::core::iop::TaggedComponent
// {

// private:

//   char* _str;

// public:

//   TIDorb::core::PolicyContext* m_policies;
 
//   ZIOPPoliciesComponent();
  
//   ZIOPPoliciesComponent(TIDorb::core::PolicyContext* policies);

//   ZIOPPoliciesComponent(const ZIOPPoliciesComponent& v);

//   ~ZIOPPoliciesComponent() throw (TIDThr::SystemException);

//   void partial_read(TIDorb::core::cdr::CDRInputStream& input);
    
//   void write(TIDorb::core::cdr::CDROutputStream& output) const;
  
//   TIDorb::core::PolicyContext* getPolicies();
    
//   const char* toString() const;

// };
// }// ziop
// }// core
// }// TIDorb
// #endif
