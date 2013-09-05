///////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/dynAny.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
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
