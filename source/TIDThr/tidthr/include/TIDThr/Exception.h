/*/////////////////////////////////////////////////////////////////////////
//
// File:        Exception.h
// Description: This file contains the Exception classes declarations.
//             
// Rel:         01.00
// Created:     September, 2001
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
//////////////////////////////////////////////////////////////////////////*/

#include "TIDThr.h"

#ifndef __TID_THR_EXCEPTION_H__
#define __TID_THR_EXCEPTION_H__

#include <pthread.h>

#ifdef TIDTHR_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#include <exception>
#include <string>

#ifdef TIDTHR_HAVE_NAMESPACE_STD
using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
using std::string;
using std::exception;
#endif

namespace TIDThr {

class Exception : public exception {

  public:

    Exception(const char* msg = "", int error = 0);

    ~Exception() throw () {}  
    
    const char* what() const throw() { return m_what.data();}
    
    virtual const char* getName() const {return "Exception";}
    const char* getReason() const {return m_reason.data();}
    int getErrno() const {return m_errno;}
 
  protected:
 
    string m_reason;
    int m_errno;
    string m_what;
};

//
// Error Class SystemException: no memory
//
class SystemException: public Exception
{
  public:
    SystemException(const char* msg = "", int error = 0)
     : Exception(msg,error){}
     
    const char* getName() const {return "SystemException";}
};

class InterruptedException : public Exception 
{
  public:
  
    InterruptedException(const char* msg = "", int error = 0)
      : Exception(msg,error){}
      
    const char* getName() const {return "InterruptedException";}
};


class IllegalArgumentException : public Exception 
{
  public:
  
    IllegalArgumentException(const char* msg = "", int error = 0)
      : Exception(msg,error){}
      
    const char* getName() const {return "IllegalArgumentException";}
};


class IllegalMutexStateException : public Exception 
{
  public:
  
    IllegalMutexStateException(const char* msg="", int error = 0)
      : Exception(msg,error){}
      
    const char* getName() const {return "IllegalMutexStateException";}
};

class IllegalConditionStateException : public Exception 
{
  public:

    IllegalConditionStateException(const char* msg="", int error = 0)
      : Exception(msg,error){}
      
    const char* getName() const {return "IllegalConditionStateException";}
};

class IllegalMonitorStateException : public Exception 
{
  public:
    IllegalMonitorStateException(const char* msg = "", int error = 0)
      : Exception(msg,error){}
      
    const char* getName() const {return "IllegalMonitorStateException";}
};

class IllegalThreadStateException : public Exception 
{
  public:

    IllegalThreadStateException(const char* msg = "", int error = 0)
      : Exception(msg,error){}
      
    const char* getName() const {return "IllegalThreadStateException";}
};

}

ostream& operator << (ostream& os, const TIDThr::Exception& ex);


#endif
