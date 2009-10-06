// ///////////////////////////////////////////////////////////////////////////
// //
// // File:        ProfileZIOP.h
// // Description: 
// // Rel:         01.00
// // Created:     March, 2008
// //
// // Revised:
// //
// // Copyright 2008 Telefonica, I+D. Printed in Spain (Europe). All Rights
// // Reserved.
// // The copyright to the software program(s) is property of Telefonica I+D.
// // The program(s) may be used and or copied only with the express written
// // consent of Telefonica I+D or in acordance with the terms and conditions
// // stipulated in the agreement/contract under which the program(s) have
// // been supplied.
// //
// ///////////////////////////////////////////////////////////////////////////

// #include "TIDorb/core/comm/ziop.h"

// #ifndef _TIDORB_CORE_COMM_ZIOP_PROFILEZIOP_H_
// #define _TIDORB_CORE_COMM_ZIOP_PROFILEZIOP_H_

// #include "TIDThr.h"

// namespace TIDorb {
// namespace core {
// namespace comm {
// namespace ziop {

// class ProfileZIOP : public TIDorb::core::iop::TaggedProfile
// {
// private:

//   TIDorb::core::comm::ziop::Version        _version;
//   TIDorb::core::iop::VectorTaggedComponent _components;

//   bool _components_extracted;
//   TIDorb::core::cdr::CDRInputStream* _profile_data;

// protected:

//   void extract_members();


// public:

//   ProfileZIOP() throw (TIDThr::SystemException);

//   // TODO
//   ProfileZIOP(const TIDorb::core::comm::ziop::Version& version)
//     throw (TIDThr::SystemException);

//   ProfileZIOP(const TIDorb::core::comm::ziop::Version& version,
//               const TIDorb::core::iop::VectorTaggedComponent& tagged_components) //copia
//     throw (TIDThr::SystemException);

//   ProfileZIOP(const TIDorb::core::comm::ziop::ProfileZIOP& other)
//     throw (TIDThr::SystemException);

//   ~ProfileZIOP() throw(TIDThr::SystemException);

//   ProfileZIOP& operator= (const TIDorb::core::comm::ziop::ProfileZIOP& other);

//   const TIDorb::core::comm::ziop::Version& getVersion() const;

//   const TIDorb::core::iop::VectorTaggedComponent& getTaggedComponents() const;

//   bool operator== (const TIDorb::core::iop::TaggedProfile& other) const;

//   void partial_read(TIDorb::core::cdr::CDRInputStream& input);

//   void partial_write(TIDorb::core::cdr::CDROutputStream& out) const;

//   char* toString() const;


// };
// }// ziop
// }// comm
// }// core
// }// TIDorb
// #endif
