///////////////////////////////////////////////////////////////////////////
//
// File:        Key.C
// Description: This file contains the Key class methods
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

#include <pthread.h>

#ifdef TIDTHR_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDTHR_HAVE_NAMESPACE_STD
using namespace std;
#endif

namespace TIDThr {

//Default Constructor: init the mutex with the default arguments
Key::Key()
    throw (SystemException)
{
    int error = pthread_key_create(&m_key, NULL);
    
    if(error){
        throw SystemException("Key::Key(): ",error);
    }
};


//Desctructor: decrements the reference count, if 0 destroys it
Key::~Key()
    throw (SystemException)
{
    int error = pthread_key_delete(m_key);
  
    if(error) {
        cerr << endl         
             << "TID-THREAD LIBRARY ERROR: Key::~Key(): pthread_key_delete() " 
             << endl;
    }
};

} // namespace TIDThr 
