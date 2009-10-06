//////////////////////////////////////////////////////////////////////////////////
//
// File:        EnviromentImpl.h
// Description: CORBA::Enviroment implementation.
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

#ifndef __TIDorb_core_EnvironmentImpl_h_
#define __TIDorb_core_EnvironmentImpl_h_

#include "CORBA.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {
  
  class EnvironmentImpl : public CORBA::Environment
                        //jagd  public TIDThr::RecursiveMutex
  {
    public:

      EnvironmentImpl()  throw (TIDThr::SystemException);
          
      ~EnvironmentImpl() throw (TIDThr::SystemException);
      
      void exception(CORBA::Exception* e);
  
      CORBA::Exception* exception() const;
      
      void clear();
 
  
      
    private:
      CORBA::Exception* m_exception;
  };

}
}

#endif
