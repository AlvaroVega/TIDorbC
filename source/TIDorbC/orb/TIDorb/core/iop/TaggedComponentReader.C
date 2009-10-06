/////////////////////////////////////////////////////////////////////////
//
// File:        TaggedComponentReader.C
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




TIDorb::core::iop::TaggedComponent* 
TIDorb::core::iop::TaggedComponentReader::read(TIDorb::core::cdr::CDRInputStream& input)
{

        CORBA::ULong profile_id;
        input.read_ulong(profile_id);

        switch(profile_id) {
                case TAG_ORB_TYPE:
                {
                  //PENDIENTE - MCPG
                  ORBComponent* orb_component = new ORBComponent();
                  orb_component->partial_read(input);
                  return orb_component;
                }

                // MIOP extensions
                case TAG_GROUP:
                {
                  TIDorb::core::comm::miop::GroupInfo* comp;
                  comp = new TIDorb::core::comm::miop::GroupInfo();
                  comp->partial_read(input);
                  return comp;
                }

                case TAG_GROUP_IIOP:
                {
                  TIDorb::core::comm::miop::GroupIIOPProfile* comp;
                  comp = new TIDorb::core::comm::miop::GroupIIOPProfile();
                  comp->partial_read(input);
                  return comp;
                }
                // end MIOP extensions

                // FT extensions
                case TAG_ALTERNATE_IIOP_ADDRESS:
                {
                  TIDorb::core::comm::FT::AlternateIIOPAddress* comp;
                  comp = new TIDorb::core::comm::FT::AlternateIIOPAddress();
                  comp->partial_read(input);
                  return comp;
                }

                case TAG_FT_HEARTBEAT_ENABLED:
                {
                  TIDorb::core::comm::FT::HeartbeatEnabled* comp;
                  comp = new TIDorb::core::comm::FT::HeartbeatEnabled();
                  comp->partial_read(input);
                  return comp;
                }
                // end FT extensions

                case TAG_POLICIES:
                {
                  TIDorb::core::messaging::PoliciesComponent* policies;
                  policies = new TIDorb::core::messaging::PoliciesComponent();
                  policies->partial_read(input);
                  return policies;
                }

                default:
                {
                  TaggedComponent* comp = new TaggedComponent(profile_id);
                  comp->partial_read(input);
                  return comp;
                }
        }
}
