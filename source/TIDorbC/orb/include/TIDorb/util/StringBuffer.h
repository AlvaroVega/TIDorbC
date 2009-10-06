/*/////////////////////////////////////////////////////////////////////////
//
// File:        StringBuffer.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////*/

#ifndef __TIDORB_UTIL_STRING_BUFFER__
#define __TIDORB_UTIL_STRING_BUFFER__

#ifdef TIDORB_HAVE_SSTREAM
#include <sstream>
#define TIDORB_OSTRINGSTREAM ostringstream
#else
#ifdef TIDORB_HAME_STRSTREAM
#include <strstream>
#else
#include <strstream.h>
#endif
#define TIDORB_OSTRINGSTREAM ostrstream
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

namespace TIDorb {
namespace util   {

typedef TIDORB_OSTRINGSTREAM StringBuffer;

}
}


#endif

