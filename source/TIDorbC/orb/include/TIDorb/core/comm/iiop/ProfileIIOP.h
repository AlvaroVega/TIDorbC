/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/ProfileIIOP.h

 Revisions:

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
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_PROFILEIIOP_H_
#define _TIDORB_CORE_COMM_IIOP_PROFILEIIOP_H_

#include "TIDThr.h"
#include "SSLIOP.h"
#include "CSIIOP.h"
#include <vector>

#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
using std::vector;
#endif

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class ProfileIIOP : public TIDorb::core::iop::TaggedProfile 
{
private:

  TIDorb::core::comm::iiop::Version           _version;
  TIDorb::core::comm::iiop::VectorListenPoint _listen_points;
  TIDorb::core::comm::iiop::ObjectKey_ref     _object_key;

  bool                                     _components_extracted;
  TIDorb::core::iop::VectorTaggedComponent _components;
	
  TIDorb::core::cdr::CDRInputStream*       _profile_data;

  TIDorb::core::PolicyContext*	           _policies;

  SSLIOP::SSL*                             _ssl;

  CSIIOP::CompoundSecMechList*             _compound_sec_mechs;

protected:
	
  //synchronized void extract_members(); 
  void extract_members();
	
	
public:

  ProfileIIOP() throw (TIDThr::SystemException);
	
  ProfileIIOP(const TIDorb::core::comm::iiop::Version& version, 
              const TIDorb::core::comm::iiop::ListenPoint& listen_point)
    throw (TIDThr::SystemException);
	
  ProfileIIOP(const TIDorb::core::comm::iiop::Version& version, 
              const TIDorb::core::comm::iiop::ListenPoint& listen_point,
              TIDorb::core::comm::iiop::ObjectKey* object_key, 
              const TIDorb::core::iop::VectorTaggedComponent& tagged_components) //copia
    throw (TIDThr::SystemException);

  ProfileIIOP(const TIDorb::core::comm::iiop::ProfileIIOP& other)
    throw (TIDThr::SystemException);

  ~ProfileIIOP() throw(TIDThr::SystemException);

  ProfileIIOP& operator= (const TIDorb::core::comm::iiop::ProfileIIOP& other);
		
  const TIDorb::core::comm::iiop::Version& getVersion() const;
	
  const TIDorb::core::comm::iiop::ListenPoint& getListenPoint() const;

  const TIDorb::core::comm::iiop::VectorListenPoint& getListenPoints() const;

  // FT extensions
  const TIDorb::core::comm::iiop::ListenPoint& getAnyListenPoint() const;
  // end FT extensions
	
  TIDorb::core::comm::iiop::ObjectKey* getObjectKey() const;
	
  const TIDorb::core::iop::VectorTaggedComponent& getTaggedComponents() const;
	
  bool operator== (const TIDorb::core::iop::TaggedProfile& other) const;

  void partial_read(TIDorb::core::cdr::CDRInputStream& input);
  	
  void partial_write(TIDorb::core::cdr::CDROutputStream& out) const;
	
  char* toString() const;

  TIDorb::core::PolicyContext* getPolicies() const;

  SSLIOP::SSL* get_SSL() const;

  CSIIOP::CompoundSecMechList* get_CompoundSecMechList() const;
	
};
}// iiop
}// comm
}// core
}// TIDorb
#endif
