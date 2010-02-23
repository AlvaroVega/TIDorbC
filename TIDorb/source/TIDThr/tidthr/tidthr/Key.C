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
