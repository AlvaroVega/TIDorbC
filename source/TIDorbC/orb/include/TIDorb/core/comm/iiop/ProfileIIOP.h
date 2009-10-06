/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/ProfileIIOP.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_PROFILEIIOP_H_
#define _TIDORB_CORE_COMM_IIOP_PROFILEIIOP_H_

#include "TIDThr.h"

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
  typedef vector<TIDorb::core::comm::iiop::ListenPoint> ListenPointList;

  TIDorb::core::comm::iiop::Version        _version;
  //TIDorb::core::comm::iiop::ListenPoint  _listen_point;
  ListenPointList                          _listen_points;
  TIDorb::core::comm::iiop::ObjectKey_ref  _object_key;

  bool _components_extracted;
  TIDorb::core::iop::VectorTaggedComponent _components;
	
  TIDorb::core::cdr::CDRInputStream* _profile_data;

  TIDorb::core::PolicyContext*	_policies;

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

  // pra@tid.es - FT extensions
  const TIDorb::core::comm::iiop::ListenPoint& getAnyListenPoint() const;
  // end FT extensions
	
  TIDorb::core::comm::iiop::ObjectKey* getObjectKey() const;
	
  const TIDorb::core::iop::VectorTaggedComponent& getTaggedComponents() const;
	
  bool operator== (const TIDorb::core::iop::TaggedProfile& other) const;

  void partial_read(TIDorb::core::cdr::CDRInputStream& input);
  	
  void partial_write(TIDorb::core::cdr::CDROutputStream& out) const;
	
  char* toString() const;

  TIDorb::core::PolicyContext* getPolicies() const;
	
};
}// iiop
}// comm
}// core
}// TIDorb
#endif
