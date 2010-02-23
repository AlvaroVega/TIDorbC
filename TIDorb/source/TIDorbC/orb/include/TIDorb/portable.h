///////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/portable.h
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

