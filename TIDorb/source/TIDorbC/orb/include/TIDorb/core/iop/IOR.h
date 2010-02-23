//////////////////////////////////////////////////////////////////////////////////
//
// File:        IOR.h
// Description: IOR implementation.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/iop.h"

#ifndef _TIDORB_CORE_IOP_IOR_H_
#define _TIDORB_CORE_IOP_IOR_H_

#include "TIDorb/util.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace iop {
 
class IOR : public TIDThr::RecursiveMutex {
private:   
	
  // IOR utils
  TIDorb::core::comm::iiop::ProfileIIOP_ref  _profile_IIOP;
  CORBA::ULong                               _profile_IIOP_position;
  CORBA::Boolean                             _profile_IIOP_searched;

  // MIOP extensions
  TIDorb::core::comm::miop::ProfileMIOP_ref  _profile_MIOP;
  CORBA::ULong                               _profile_MIOP_position;
  CORBA::Boolean                             _profile_MIOP_searched;
  // end MIOP extensions

  CORBA::Boolean                             _ZIOP_searched;
  CORBA::Boolean                             _has_ZIOP_policies;

  static IOR_ref _null_ior;  

  VectorTaggedProfile _profiles;
  
  char*  _type_id;
  char* _ior_string;
    
  bool has_hash_code;
  CORBA::ULong hash_code;
	    
public: 

  IOR() throw (TIDThr::SystemException);
  ~IOR() throw (TIDThr::SystemException);
    
  IOR(const char* id, const VectorTaggedProfile* profiles) throw (TIDThr::SystemException);
  
  static IOR* null_ior();
  
  void assign(const TIDorb::core::iop::IOR& ior);
    
  const char* get_type_id() const;
  
  CORBA::ULong member_count() const;
  
  TIDorb::core::iop::TaggedProfile* get_profile(CORBA::ULong index) const;
    
  bool equivalent(const TIDorb::core::iop::IOR& ior) const;
    
  const char* toString() const;
  
  CORBA::ULong hashCode() const;
  
  const char* toString(TIDorb::core::TIDORB* orb) const;
  
  static IOR* fromString(TIDorb::core::TIDORB* orb, const char* ior_str);
  
  void write(TIDorb::core::cdr::CDROutputStream& out) const;
  
  void read(TIDorb::core::cdr::CDRInputStream& input);
  
  static void skip(TIDorb::core::cdr::CDRInputStream& input);
  
  IOR& operator=(TIDorb::core::iop::IOR* ior) throw (TIDThr::SystemException);
  
  IOR* getIOR() const
  {
    return (IOR*)this;
  }

  TIDorb::core::comm::iiop::ProfileIIOP* profile_IIOP() const;
  
  CORBA::ULong profile_IIOP_position() const {
    return _profile_IIOP_position;
  }
  
  bool is_IIOP() const;

  bool use_another_Profile_IIOP();

  // MIOP extensions
  TIDorb::core::comm::miop::ProfileMIOP* profile_MIOP() const;
  const TIDorb::core::comm::iiop::Version& GIOPVersion() const;
  bool is_group_reference() const;
  // end MIOP extensions

  bool is_ZIOP();
  TIDorb::core::comm::iiop::ObjectKey* object_key() const;

  TIDorb::core::PolicyContext* policies() const;

};
}// iop
}// core
}// TIDorb
#endif

