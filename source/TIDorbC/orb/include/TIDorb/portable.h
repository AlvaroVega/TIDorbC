///////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/portable.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
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

#include "TIDorb/config.h"
#include "CORBA.h"

#ifndef _TIDORB_portable_H_
#define _TIDORB_portable_H_

#include "TIDThr.h"

// Forward Declarations

namespace TIDorb {
namespace portable {

class RefCounter;
class Any;
class InputStream;
class OutputStream;
class Streamable;
class Delegate;
class TypeCodeFactory;
class Stub;
class ORB;

}
}

#include "TIDorb/portable/RefCounter.h"
#include "TIDorb/portable/Any.h"
#include "TIDorb/portable/InputStream.h"
#include "TIDorb/portable/OutputStream.h"
#include "TIDorb/portable/Streamable.h"
#include "TIDorb/portable/ObjectDelegate.h"

#endif

