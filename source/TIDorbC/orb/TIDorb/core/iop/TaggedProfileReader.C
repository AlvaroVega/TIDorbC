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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
