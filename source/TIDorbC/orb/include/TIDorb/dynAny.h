///////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/dynAny.h
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

#ifndef __TIDORB_DYN_ANY_H__
#define __TIDORB_DYN_ANY_H__

#include "TIDorb/config.h"
#include "DynamicAny.h"
#include "TIDorb/core.h"

namespace TIDorb {
namespace dynAny {

// Forward declarations
class DynAnyFactoryImpl;

class DynAnyBase;
class DynSingleton;
class DynComposite;
class DynAnyImpl;
class DynFixedImpl;
class DynEnumImpl;
class DynStructImpl;
class DynUnionImpl;
class DynSequenceImpl;
class DynArrayImpl;
class DynValueImpl;
class DynExceptImpl;

}}


#include "TIDorb/dynAny/DynAnyBase.h"
#include "TIDorb/dynAny/DynSingleton.h"

#include "TIDorb/dynAny/DynAnyImpl.h"

#include "TIDorb/dynAny/DynComposite.h"

#include "TIDorb/dynAny/DynFixedImpl.h"
#include "TIDorb/dynAny/DynEnumImpl.h"
#include "TIDorb/dynAny/DynStructImpl.h"

#include "TIDorb/dynAny/DynExceptImpl.h"

#include "TIDorb/dynAny/DynUnionImpl.h"
#include "TIDorb/dynAny/DynSequenceImpl.h"
#include "TIDorb/dynAny/DynArrayImpl.h"
#include "TIDorb/dynAny/DynValueImpl.h"


#include "TIDorb/dynAny/DynAnyFactoryImpl.h"


#endif //__TIDORB_DYN_ANY_H__
