//////////////////////////////////////////////////////////////////////////////////
//
// File:        iop_fw.h
// Description: 
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

#include "TIDorb/core_fw.h"

#ifndef _TIDORB_CORE_IOP_FW_H_
#define _TIDORB_CORE_IOP_FW_H_


#include <vector>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::vector;
#endif


// Forward Declarations

namespace TIDorb {
namespace core {
namespace iop {
  
  class TaggedComponent;
  typedef TIDThr::HandleT<TaggedComponent> TaggedComponent_ref;
  typedef vector<TaggedComponent_ref> VectorTaggedComponent;
  
  class TaggedProfile;
  typedef TIDThr::HandleT<TaggedProfile> TaggedProfile_ref;
  typedef vector<TaggedProfile_ref> VectorTaggedProfile;
  	 
  class TaggedComponentReader;
  class ORBComponent;
  
  class ProfileMultipleComponents;
  class TaggedProfileReader;
  
  class IOR;
  typedef TIDThr::HandleT<IOR> IOR_ref;
  
}
}
}


#endif
