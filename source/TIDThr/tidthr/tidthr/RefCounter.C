///////////////////////////////////////////////////////////////////////////
//
// File:        TIDThr_RefCounter.C
// Description: This file contains the RefCounter class methods
// Rel:         01.00
// Created:     May, 2001
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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

#include "TIDThr.h"

namespace TIDThr {

RefCounter::RefCounter() throw (SystemException)
{ 
  __ref_count_ = 0;
  int ret = pthread_mutex_init(&__ref_mutex_,NULL);
  if(ret)
    throw SystemException("RefCounter: pthread_mutex_init",ret);
}

RefCounter::~RefCounter() throw (SystemException)
{
  pthread_mutex_destroy(&__ref_mutex_);
}

} // namespace TIDThr 
