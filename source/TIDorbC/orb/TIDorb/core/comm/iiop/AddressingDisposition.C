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
