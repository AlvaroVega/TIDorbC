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




TIDorb::core::iop::TaggedComponent* 
TIDorb::core::iop::TaggedComponentReader::read(TIDorb::core::cdr::CDRInputStream& input)
{

        CORBA::ULong profile_id;
        input.read_ulong(profile_id);

        switch(profile_id) {
                case TAG_ORB_TYPE:
                {
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

                case TAG_SSL_SEC_TRANS:
                {
                  TIDorb::core::comm::ssliop::SSLComponent* ssl_component;
                  ssl_component = new TIDorb::core::comm::ssliop::SSLComponent();
                  ssl_component->partial_read(input);
                  return ssl_component;
                }

                case TAG_CSI_SEC_MECH_LIST:
                {
                  TIDorb::core::security::CSIComponent* csi_component;
                  csi_component = new TIDorb::core::security::CSIComponent();
                  csi_component->partial_read(input);
                  return csi_component;
                }

                default:
                {
                  TaggedComponent* comp = new TaggedComponent(profile_id);
                  comp->partial_read(input);
                  return comp;
                }
        }
}
