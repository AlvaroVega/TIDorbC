///////////////////////////////////////////////////////////////////////////
//
// File:        ProfileMIOP.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
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
