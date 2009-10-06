///////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/templates_fw.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
//
// Revised:
//
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////

#ifndef _TIDORB_templates_fw_H_
#define _TIDORB_templates_fw_H_ 

#include "TIDorb/config.h"


namespace TIDorb {
namespace templates {

  template<class T>  class InterfaceT_out;
  template<class InterfaceT> class InterfaceT_ptr_SequenceMember;
  template<class InterfaceT> class InterfaceT_var;
  template<class T> class RefCount_out;
  template<class T> class RefCount_var;
  template<typename T, typename ContentT> class SequenceT_out;
  template<typename T, typename ContentT > class SequenceT_var;
  template<typename T> class FixedSizeT_var;
  template<typename T> class VariableSizeT_out;
  template<typename T> class VariableSizeT_var;
  template<class T> class ValuetypeT_out;
  template<class ValuetypeT> class ValuetypeT_ptr_SequenceMember;
  template<class T> class ValuetypeT_var;

}
}


#endif
