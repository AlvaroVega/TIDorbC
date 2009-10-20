//////////////////////////////////////////////////////////////////////////////////
//
// File:        util.h
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

#ifndef _TIDORB_CORE_UTIL_H_
#define _TIDORB_CORE_UTIL_H_

namespace TIDorb {
namespace core {
namespace util {
	
	class BooleanHolder;
	class CharHolder;
	class DoubleHolder;
	class FloatHolder;
	class LongDoubleHolder;
	class LongHolder;
	class LongLongHolder;
	class OctetHolder;
	class ShortHolder;
	class StringHolder;
	class ULongHolder;
	class ULongLongHolder;
	class UShortHolder;
	class WCharHolder;
	class WStringHolder;
	class FixedHolder;
	class AnyHolder;
	class ObjectHolder;
	class ValueHolder;
	class TypeCodeHolder;
	class SystemExceptionHolder;
	class Counter;
	class RemovableObject;
	class OperationCompletion;
	struct OnlyOneThreadCanWait;

	class Octet;
	class Short;
	class UShort;
	class Long;
	class ULong;
	class LongLong;
	class ULongLong;
	class Float;
	class Double;
	class LongDouble;

    class Time;
}
}
}

#include "TIDorb/core/util/InitialReference.h"

#include "TIDorb/core/util/URL.h"
#include "TIDorb/core/util/Corbaname.h"

#include "TIDorb/core/util/BooleanHolder.h"
#include "TIDorb/core/util/CharHolder.h"
#include "TIDorb/core/util/DoubleHolder.h"
#include "TIDorb/core/util/FloatHolder.h"
#include "TIDorb/core/util/LongDoubleHolder.h"
#include "TIDorb/core/util/LongHolder.h"
#include "TIDorb/core/util/LongLongHolder.h"
#include "TIDorb/core/util/OctetHolder.h"
#include "TIDorb/core/util/ShortHolder.h"
#include "TIDorb/core/util/StringHolder.h"
#include "TIDorb/core/util/ULongHolder.h"
#include "TIDorb/core/util/ULongLongHolder.h"
#include "TIDorb/core/util/UShortHolder.h"
#include "TIDorb/core/util/WCharHolder.h"
#include "TIDorb/core/util/WStringHolder.h"
#include "TIDorb/core/util/FixedHolder.h"
#include "TIDorb/core/util/AnyHolder.h"
#include "TIDorb/core/util/ObjectHolder.h"
#include "TIDorb/core/util/ValueHolder.h"
#include "TIDorb/core/util/OnlyOneThreadCanWait.h"
#include "TIDorb/core/util/OperationCompletion.h"
#include "TIDorb/core/util/TypeCodeHolder.h"
#include "TIDorb/core/util/Counter.h"

#include "TIDorb/core/util/RemovableObject.h"
#include "TIDorb/core/util/UseTable.h"


#include "TIDorb/core/util/CORBAException.h"
#include "TIDorb/core/util/exceptions.h"
#include "TIDorb/core/util/SystemExceptionHolder.h"

#include "TIDorb/core/util/Octet.h"
#include "TIDorb/core/util/Short.h"
#include "TIDorb/core/util/UShort.h"
#include "TIDorb/core/util/Long.h"
#include "TIDorb/core/util/ULong.h"
#include "TIDorb/core/util/LongLong.h"
#include "TIDorb/core/util/ULongLong.h"
#include "TIDorb/core/util/Float.h"
#include "TIDorb/core/util/Double.h"
#include "TIDorb/core/util/LongDouble.h"

#include "TIDorb/core/util/Time.h"

#endif
