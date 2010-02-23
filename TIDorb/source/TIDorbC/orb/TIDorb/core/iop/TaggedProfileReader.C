/////////////////////////////////////////////////////////////////////////
//
// File:        TaggedProfileReader.C
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

#include "TIDorb/core.h"




TIDorb::core::iop::TaggedProfile*
TIDorb::core::iop::TaggedProfileReader::read(TIDorb::core::cdr::CDRInputStream& input)
{
        CORBA::ULong profile_id;
        input.read_ulong(profile_id);

        TIDorb::core::comm::iiop::ProfileIIOP* profile_iiop;
        ProfileMultipleComponents*             multi_comp;

        // pra@tid.es - MIOP extensions
        TIDorb::core::comm::miop::ProfileMIOP* profile_miop;
        // end MIOP extensions

        switch(profile_id) {
                case TAG_INTERNET_IOP:
                        profile_iiop = new TIDorb::core::comm::iiop::ProfileIIOP();
                        profile_iiop->partial_read(input);
                        return (TaggedProfile*) profile_iiop;

                case TAG_MULTIPLE_COMPONENTS:
                        multi_comp = new TIDorb::core::iop::ProfileMultipleComponents();
                        multi_comp->partial_read(input);
                        return (TaggedProfile*) multi_comp;

                // pra@tid.es - MIOP extensions
                case TAG_UIPMC:
                        profile_miop = new TIDorb::core::comm::miop::ProfileMIOP();
                        profile_miop->partial_read(input);
                        return (TaggedProfile*) profile_miop;
                // end MIOP extensions

                default:
                        throw CORBA::MARSHAL("Invalid Profile TAG.", 0, CORBA::COMPLETED_NO);
        }
}
