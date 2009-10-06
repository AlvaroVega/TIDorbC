/*/////////////////////////////////////////////////////////////////////////
//
// File:        Key.h
// Description: This file contains the Key class declaration for thread-specific memory allocation.
//             
// Rel:         01.00
// Created:     September, 2001
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
/////////////////////////////////////////////////////////////////////////*/

#include "TIDThr.h"

#ifndef __TID_THR_KEY_H__
#define __TID_THR_KEY_H__

#include <pthread.h>

namespace TIDThr {

class Key : public  RefCounter {
 
  public:

    //Default Constructor:
    Key() throw (SystemException);
    
   //Desctructor: decrements the reference count, if 0 destroys it
    virtual ~Key() throw (SystemException);
  
    pthread_key_t theKey(){return m_key;}
    
  protected:
    // Posix Key descriptor
    pthread_key_t m_key; 
};

} // namespace

#endif


