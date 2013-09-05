/////////////////////////////////////////////////////////////////////////
//
// File:        TaggedProfile.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/iop/TaggedProfile.h"




TIDorb::core::iop::TaggedProfile::TaggedProfile(CORBA::ULong tag)
        throw (TIDThr::SystemException)
{
        _tag = tag;
}




void TIDorb::core::iop::TaggedProfile::write(TIDorb::core::cdr::CDROutputStream& output) const
{
        // write tag
        output.write_ulong(_tag);

        // write profile data
        partial_write(output);
}




void TIDorb::core::iop::TaggedProfile::skip(TIDorb::core::cdr::CDRInputStream& input)
{
        // skip tag
        input.skip_ulong();

        // skip encapsulation
        input.skip_encapsulation();
}
