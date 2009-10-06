//////////////////////////////////////////////////////////////////////////////////
//
// File:        cdr_fw.h
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

#ifndef _TIDORB_CORE_CDR_FW_H_
#define _TIDORB_CORE_CDR_FW_H_

#include "TIDThr.h"

// Forward Declarations

namespace TIDorb {
namespace core {
namespace cdr {
  
  class CDR;
  class PointerCDR;
  class ChunkCDR;
  class BufferCDR;
  typedef TIDThr::HandleT<BufferCDR> BufferCDR_ref;
  
  class ContextCDR;  
  class ValueTypeInfo;  
  class IndirectionNode;
  class PositionsNode;
  class IteratorCDR;
  class CDROutputStream;
  class CDRInputStream;
}
}
}

#include "TIDorb/core/cdr/CDR.h"

#endif
