//////////////////////////////////////////////////////////////////////////////////
//
// File:        ForwardRequest.h
// Description: ForwardRequest
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

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_FORWARD_REQUEST_H_
#define _TIDORB_CORE_FORWARD_REQUEST_H_


#include <exception>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
using std::exception;
#endif


namespace TIDorb {
namespace core {


class ForwardRequest : public exception
{
  
  public:
    TIDorb::core::iop::IOR_ref m_reference;
  
    ForwardRequest(TIDorb::core::iop::IOR* ref) throw()
    {
      m_reference = ref;
    }

    ~ForwardRequest() throw(){}
    
    TIDorb::core::iop::IOR* reference() const
    {
      return m_reference;
    }
  
};

}
}

#endif

