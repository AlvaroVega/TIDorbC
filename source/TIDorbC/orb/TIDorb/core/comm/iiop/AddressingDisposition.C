//////////////////////////////////////////////////////////////////////////////////
//
// File:        AddressingDisposition.C
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

#include "TIDorb/core/comm/iiop/AddressingDisposition.h"




TIDorb::core::comm::iiop::AddressingDisposition
  TIDorb::core::comm::iiop::AddressingDispositionHelper::from_ushort(CORBA::UShort value)
{
        if(value <= ReferenceAddr)
          return (AddressingDisposition) value;
        else
          throw CORBA::MARSHAL("Invalid AddressingDisposition value");
}




TIDorb::core::comm::iiop::AddressingDisposition
  TIDorb::core::comm::iiop::AddressingDispositionHelper::read(TIDorb::portable::InputStream& input)
{
  CORBA::UShort value;
  input.read_ushort(value);
  return AddressingDispositionHelper::from_ushort(value);
}




void TIDorb::core::comm::iiop::AddressingDispositionHelper::write
  (TIDorb::portable::OutputStream& output,
   TIDorb::core::comm::iiop::AddressingDisposition value)
{
  output.write_ushort((CORBA::UShort) value);
}
