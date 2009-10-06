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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
