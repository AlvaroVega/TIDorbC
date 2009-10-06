///////////////////////////////////////////////////////////////////////////
//
// File:        ProfileMIOP.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
//
// Revised:
//
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/miop.h"

#ifndef _TIDORB_CORE_COMM_MIOP_PROFILEMIOP_H_
#define _TIDORB_CORE_COMM_MIOP_PROFILEMIOP_H_

#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace comm {
namespace miop {

class ProfileMIOP : public TIDorb::core::iop::TaggedProfile
{
private:

  TIDorb::core::comm::miop::Version        _version;
  TIDorb::core::comm::miop::ListenPoint    _listen_point;
  TIDorb::core::iop::VectorTaggedComponent _components;

  bool _components_extracted;
  TIDorb::core::cdr::CDRInputStream* _profile_data;

protected:

  //synchronized void extract_members();
  void extract_members();


public:

  ProfileMIOP() throw (TIDThr::SystemException);

  ProfileMIOP(const TIDorb::core::comm::miop::Version& version,
              const TIDorb::core::comm::miop::ListenPoint& listen_point)
    throw (TIDThr::SystemException);

  ProfileMIOP(const TIDorb::core::comm::miop::Version& version,
              const TIDorb::core::comm::miop::ListenPoint& listen_point,
              const TIDorb::core::iop::VectorTaggedComponent& tagged_components) //copia
    throw (TIDThr::SystemException);

  ProfileMIOP(const TIDorb::core::comm::miop::ProfileMIOP& other)
    throw (TIDThr::SystemException);

  ~ProfileMIOP() throw(TIDThr::SystemException);

  ProfileMIOP& operator= (const TIDorb::core::comm::miop::ProfileMIOP& other);

  const TIDorb::core::comm::miop::Version& getVersion() const;

  const TIDorb::core::comm::miop::ListenPoint& getListenPoint() const;

  const TIDorb::core::iop::VectorTaggedComponent& getTaggedComponents() const;

  bool operator== (const TIDorb::core::iop::TaggedProfile& other) const;

  void partial_read(TIDorb::core::cdr::CDRInputStream& input);

  void partial_write(TIDorb::core::cdr::CDROutputStream& out) const;

  char* toString() const;

};
}// miop
}// comm
}// core
}// TIDorb
#endif
