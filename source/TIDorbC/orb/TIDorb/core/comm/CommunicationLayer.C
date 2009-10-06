/////////////////////////////////////////////////////////////////////////
//
// File:        CommunicationLayer.C
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

#include "TIDorb/core/comm.h"


TIDorb::core::comm::CommunicationLayer::CommunicationLayer(TIDorb::core::TIDORB* orb)
  throw (TIDThr::SystemException)
  : _orb(orb), conf(orb->conf())
{
}
