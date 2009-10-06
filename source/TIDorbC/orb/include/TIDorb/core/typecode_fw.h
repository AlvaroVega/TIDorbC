//////////////////////////////////////////////////////////////////////////////////
//
// File:        typecode_fw.h
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#ifndef _TIDORB_CORE_TYPECODE_FW_H_
#define _TIDORB_CORE_TYPECODE_FW_H_

// Forward Declarations

namespace TIDorb {
namespace core {
namespace typecode {
  
  class TypeCodeFactory;
  class TypeCodeMarshaler;
  class TypeCodeImpl;
  class FixedTypeCode;
  class StringTypeCode;
  class WStringTypeCode;
  class ArrayTypeCode;
  class SequenceTypeCode;
  class ComplexTypeCode;
  class AliasTypeCode;
  class StructTypeCode;
  class ExceptionTypeCode;
  class EnumTypeCode;
  class ObjectRefTypeCode;
  class UnionTypeCode;
  class NativeTypeCode;
  class AbstractInterfaceTypeCode;
  class LocalInterfaceTypeCode;
  class ValueTypeCode;
  class ValueBoxTypeCode;
  class RecursiveTypeCode;
    
}
}
}

#endif
