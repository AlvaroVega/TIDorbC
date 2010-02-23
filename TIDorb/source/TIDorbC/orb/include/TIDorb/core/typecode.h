//////////////////////////////////////////////////////////////////////////////////
//
// File:        typecode.h
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_TYPECODE_H_
#define _TIDORB_CORE_TYPECODE_H_
#include "TIDorb/core/typecode/TypeCodeMarshaler.h"
#include "TIDorb/core/typecode/TypeCodeFactory.h"
#include "TIDorb/core/typecode/TypeCodeImpl.h"
#include "TIDorb/core/typecode/FixedTypeCode.h"
#include "TIDorb/core/typecode/StringTypeCode.h"
#include "TIDorb/core/typecode/WStringTypeCode.h"
#include "TIDorb/core/typecode/ArrayTypeCode.h"
#include "TIDorb/core/typecode/SequenceTypeCode.h"
#include "TIDorb/core/typecode/ComplexTypeCode.h"
#include "TIDorb/core/typecode/AliasTypeCode.h"
#include "TIDorb/core/typecode/StructTypeCode.h"
#include "TIDorb/core/typecode/ExceptionTypeCode.h"
#include "TIDorb/core/typecode/EnumTypeCode.h"
#include "TIDorb/core/typecode/ObjectRefTypeCode.h"
#include "TIDorb/core/typecode/UnionTypeCode.h"
#include "TIDorb/core/typecode/NativeTypeCode.h"
#include "TIDorb/core/typecode/AbstractInterfaceTypeCode.h"
#include "TIDorb/core/typecode/LocalInterfaceTypeCode.h"
#include "TIDorb/core/typecode/ValueTypeCode.h"
#include "TIDorb/core/typecode/ValueBoxTypeCode.h"
#include "TIDorb/core/typecode/RecursiveTypeCode.h"
#endif // _TIDORB_CORE_TYPECODE_H_

