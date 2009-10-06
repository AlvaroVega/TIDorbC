///////////////////////////////////////////////////////////////////////////
//
// File:        Exception.C
// Description: This file contains the Exception class methods
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

#include <string.h>

namespace TIDThr {
  
Exception::Exception(const char* msg, int error)
    : m_reason(msg), m_errno(error) 
{ 
    if (strncmp(msg,"",1)!=0) {
        m_what = msg + string(" ");
    }

    if (error != 0) {
        m_what += string(strerror(error)) + string(" ") + string("[errno=");
        m_what += string(enostr(error)) + string("]");
    }  
}
  
} // namespace TIDThr



ostream& 
operator<< (ostream& os, const TIDThr::Exception& ex)
{
    os << ex.getName() << ": " << ex.what() << endl;
    return os;
};
